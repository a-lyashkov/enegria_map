#ifndef _MAP_REGS_H_

#define _MAP_REGS_H_

#define EPROM_START	0x0
#define EPROM_SIZE	0x3FF

#define RAM_START	0x400
/* XXX should from ROM */
#define RAM_SIZE	0x3FF

enum MAP_ROM_ADDR {
    MAP_DEVICE	= 0x0,
    MAP_HW_VER	= 0x1,
    MAP_FW_VER	= 0x2,
    MAP_POW	= 0x5,
    MAP_BAT_UAC	= 0x6,
    MAP_RAM_HI	= 0x9,
    MAP_RAM_LO	= 0x8,
    MAP_SERIAL_0 = 0x18,
    MAP_SERIAL_1 = 0x19,
    MAP_SERIAL_2 = 0x22,
    MAP_SERIAL_3 = 0x23,
};

#endif
