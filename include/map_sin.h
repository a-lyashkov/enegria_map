#ifndef _MAP_SIN_H_
#define _MAP_SIN_H_

#include <stdint.h>
#include <stdbool.h>

enum map_pow {
    MAP_POW_1_3	= 0,
    MAP_POW_1_5	= 1,
    MAP_POW_2	= 2,
    MAP_POW_3	= 3,
    MAP_POW_4_5	= 4,
    MAP_POW_6	= 5,
    MAP_POW_9	= 6,
    MAP_POW_12	= 7,
    MAP_POW_15	= 8,
    MAP_POW_18	= 9,
    MAP_POW_24	= 10,
    MAP_POW_36	= 11,
};

enum map_uac_bat {
    MAP_BAT_12	= 0,
    MAP_BAT_24	= 1,
    MAP_BAT_48	= 2,
    MAP_BAT_96	= 3,
};


struct map_hw_info {
    uint8_t	hw_ver;   // HW revision
    bool	hybrid;   // is hybrid ?
    uint16_t	fw_ver;   // FW revision
    enum map_pow pow;	  // Max Power
    enum map_uac_bat bat; // Bat UAC
    uint16_t	ram_high; // high address of ram
    uint32_t	serial;	  // device serial NO
};

#define MAP_FW_VERSION(maj, min)	((maj)<<8 | (min))
#define MAP_FW_MAJOR(ver)	((ver) >> 8)
#define MAP_FW_MINOR(ver)	((ver) & 0xFF)

extern int map_epprom_read(int fd);

const char *map_bat_str(enum map_uac_bat bat);
const char *map_pow_str(enum map_pow pow);

/* return error if epprom don't read before */
extern int map_hw_info(struct map_hw_info *info);

#endif
