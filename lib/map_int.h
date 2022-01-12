#ifndef _MAP_INT_H_
#define _MAP_INT_H_

#include <stdint.h>

#define EPROM_START	0x0
#define EPROM_SIZE	0x3FF

#define RAM_START	0x400
/* XXX should from ROM */
#define RAM_SIZE	0x3FF

#define conv_u16(h,l)	(((h) << 8) + l)
#define conv_ram_u16(h,l)	((map_ram[(h)] << 8) + map_ram[(l)])

#define BIT(a)	(1 << (a))


extern uint16_t fw_ver; // needs for conditional

extern uint8_t *epprom;
bool epprom_valid; // set this to false to refresh ROM

extern int map_epprom_read(int fd);

extern uint8_t *map_ram;
bool ram_valid; // set this to false to refresh ROM

extern int map_ram_read(int fd);

#endif