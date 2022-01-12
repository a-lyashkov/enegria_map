#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

#include "map_int.h"
#include "map_rom_regs.h"
#include "map_sin_rom.h"

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

int map_hw_info(int fd, struct map_hw_info *info)
{
	if (!epprom_valid) {
		if (map_epprom_read(fd) < 0)
			return -EIO;
	}

	if (!epprom_valid)
		return -EINVAL;

	info->hybrid	= epprom[MAP_HW_VER] & 0x80;
	info->hw_ver	= epprom[MAP_HW_VER] & 0x3F;
	fw_ver		=((epprom[MAP_FW_VER] & 0x1F) << 8) | (epprom[MAP_FW_VER] >> 5);
	info->fw_ver	= fw_ver;

	info->pow	= epprom[MAP_POW];
	info->bat	= epprom[MAP_BAT_UAC];
	info->ram_high	= epprom[MAP_RAM_HI] << 8 | epprom[MAP_RAM_LO];

	info->serial	= epprom[MAP_SERIAL_0] | (epprom[MAP_SERIAL_1] << 8);
	info->serial   |= (epprom[MAP_SERIAL_2] << 16) | (epprom[MAP_SERIAL_3] << 24);

	return 0;
}
