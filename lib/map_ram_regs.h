#ifndef _MAP_RAM_REGS_H_

#define _MAP_RAM_REGS_H_

enum MAP_RAM {
	// Режим работы МАП
	MAP_MODE	= 0x000,
	//  Показывает состояние заряда в котором находится МАП.
	MAP_StatusC	= 0x002,
	// Была ли запись в epprom - нулить после чтения.
	MAP_put_eeprom	= 0x003,
	// Состояние напряжения на АКБ
	MAP_StateUAcc	= 0x004,
	// Напряжение АКБ
	MAP_UAcc_med_VH = 0x005,
	MAP_UAcc_med_VL = 0x006,

	// Input power
	// UAC
	MAP_UNET	= 0x022,
	// I
	MAP_INET	= 0x023,
	// POW low
	MAP_PNET_L	= 0x024,
	// POW High
	MAP_PNET_H	= 0x057,
	//
};


#endif
