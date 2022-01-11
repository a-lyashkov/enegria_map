#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

#include "map_sin.h"
#include "map_regs.h"

enum MAP_SIN_CMD {
    READ_CMD  = 'r',
    WRITE_CMD = 'w',
    ERROR_CMD = 'e',
    OUT_CMD   = 'o',
};

#define MAP_IO_BULK 256

/**
 io protocol

send: [r/w cmd][payload size (max 256 /0-255/)][addr_high][addr_low][data][checksum]
recv: [ e | o ] [ payload ] [ checksum ]
*/

static int
map_send_echo(int fd, uint8_t d)
{
	uint8_t data = d;
	uint8_t echo;
	int rc;

	rc = write(fd, &data, sizeof(data));
	if (rc != sizeof(data))
		return -EIO;

	rc = read(fd, &echo, sizeof(data));
	if (rc != sizeof(data))
		return -EIO;

	if (data != echo)
		return -EIO;

	return 0;
}

static int
map_recv_echo(int fd, uint8_t *data)
{
	int rc;

	rc = read(fd, data, sizeof(*data));
	if (rc != sizeof(*data))
		return -EIO;

	rc = write(fd, data, sizeof(*data));
	if (rc != sizeof(*data))
		return -EIO;

	return 0;

}

static int
map_send_char(int fd, uint8_t data, uint8_t *sum)
{
	int rc;

	if (data == '\n') {
		*sum += 0xDB + 0xDC;

		rc = map_send_echo(fd, 0xDB);
		if (rc)
			return rc;

		return map_send_echo(fd, 0xDC);
	}

	if (data == 0xDB) {
		*sum += 0xDB + 0xDD;

		rc = map_send_echo(fd, 0xDB);
		if (rc)
			return rc;

		return map_send_echo(fd, 0xDD);
	}

	*sum += data;
	return map_send_echo(fd, data);
}

// checksum from data + checksum itself == 0
// if checksum != \n - extra \n needs
static int
map_send_csum(int fd, uint8_t sum)
{
	uint16_t s = 0x100 - sum;
	int rc;

	rc = map_send_echo(fd, s);
	if (rc)
		return rc;

	if (s !=  '\n')
		rc = map_send_echo(fd, '\n');

	return rc;
}

static int
map_send_cmd(int fd, enum MAP_SIN_CMD cmd, uint16_t addr, uint16_t size, void *ptr)
{
	int rc;
	uint8_t sum = 0;

	rc = map_send_char(fd, cmd, &sum);
	if (rc)
		return rc;

	/* -1 to convert into index */
	size -= 1;
	rc = map_send_char(fd, size, &sum);
	if (rc)
		return rc;

	rc = map_send_char(fd, addr >> 8, &sum);
	if (rc)
		return rc;

	rc = map_send_char(fd, addr & 0xFF, &sum);
	if (rc)
		return rc;

	if (ptr) {
		/* XXX loop */
	}

	return map_send_csum(fd, sum);
}

static int
map_recv_char(int fd, uint8_t *data, uint8_t *csum)
{
	int rc;
	uint8_t code;

	rc = map_recv_echo(fd, &code);
	if (rc)
		return rc;

	*csum += code;

	/* Special code - second byte analyze needs */
	if (code == 0xDB) {
		uint8_t	_code;

		rc = map_recv_echo(fd, &_code);
		if (rc)
			return rc;

		*csum += _code;

		switch(_code) {
		case 0xDC:
			*data = 0x0A;
			break;
		case 0xDD:
			*data = 0xDB;
			break;
		default:
			printf("invalid second code %#0x\n", _code);
			return -EPROTO;
		}
	} else {
		    *data = code;
	}

	return 0;
}

static int
map_read_answer(int fd, uint8_t *data, uint16_t size)
{
	uint8_t cmd;
	uint8_t sum = 0;
	uint8_t _sum; // csum from MAP
	uint8_t ecode;
	unsigned int i;
	int rc;

	rc = map_recv_char(fd, &cmd, &sum);
	if (rc)
		return rc;

	if (cmd == ERROR_CMD) {
		rc = map_recv_char(fd, &ecode, &sum);
		if (rc)
			return rc;
		goto csum;
	}
	if (cmd != OUT_CMD)
		return -EPROTO;

	// bulk data
	for(i=0; i < size; i++) {
		rc = map_recv_char(fd, data, &sum);
		if (rc)
			return rc;

		data ++;
	}

csum:
	rc = map_recv_char(fd, &_sum, &sum);
	if (rc)
		return rc;


	if (_sum != '\n') {
		uint8_t echo;
		uint8_t	e1;

		rc = map_recv_char(fd, &echo, &e1);
		if (rc)
			return rc;
	}

	if (sum != 0)
		return -EPROTO;

	return cmd == ERROR_CMD ? -(512+ecode) : 0;
}

static int
map_io_read(int fd, uint8_t *data, uint16_t addr, uint16_t size)
{
	int rc;
	uint16_t pos;
	uint16_t sz;

	tcdrain(fd);
	for(pos = 0; pos < size; pos += MAP_IO_BULK) {
	    sz = size - pos;
	    sz = sz > MAP_IO_BULK ? MAP_IO_BULK : sz;

	    rc = map_send_cmd(fd, READ_CMD, addr + pos, sz, NULL);
	    if (rc < 0)
		    return rc;

	    rc = map_read_answer(fd, data + pos, sz);
	    if (rc < 0)
		    return rc;
	}

	return 0;
}

uint16_t fw_ver; // needs for conditional
uint16_t ram_high; // maximal ram address

static uint8_t *epprom;
static bool epprom_valid = false;

int map_epprom_read(int fd)
{
	int rc = 0;

	if (epprom == NULL) {
		epprom = malloc(EPROM_SIZE);
	}
	if (epprom == NULL)
		return -ENOMEM;

	epprom_valid = false;
	rc = map_io_read(fd, epprom, EPROM_START, EPROM_SIZE);
	if (rc < 0)
		return rc;

	epprom_valid = true;

	return 0;
}

static char *_map_bat_str[] = {
    "12V", "24V", "48V", "96V"
};

const char *map_bat_str(enum map_uac_bat bat)
{
	return _map_bat_str[bat];
}

static char *_map_pow_str[] = {
"1.3kW", "1.5kW", "2kW", "3kW", "4.5kW", "6kW",
"9kW", "12kW", "15kW", "18kW", "24kW", "36kW"
};

const char* map_pow_str(enum map_pow pow)
{
	return _map_pow_str[pow];
}

int map_hw_info(struct map_hw_info *info)
{
	if (!epprom_valid)
		return -EINVAL;

	info->hybrid	= epprom[MAP_HW_VER] & 0x80;
	info->hw_ver	= epprom[MAP_HW_VER] & 0x3F;
	fw_ver		=((epprom[MAP_FW_VER] & 0x1F) << 8) | (epprom[MAP_FW_VER] >> 5);
	info->fw_ver	= fw_ver;

	info->pow	= epprom[MAP_POW];
	info->bat	= epprom[MAP_BAT_UAC];
	ram_high	= epprom[MAP_RAM_HI] << 8 | epprom[MAP_RAM_LO];
	info->ram_high	= ram_high;

	info->serial	= epprom[MAP_SERIAL_0] | (epprom[MAP_SERIAL_1] << 8);
	info->serial   |= (epprom[MAP_SERIAL_2] << 16) | (epprom[MAP_SERIAL_3] << 24);

	return 0;
}
