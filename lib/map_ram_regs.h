#ifndef _MAP_RAM_REGS_H_

#define _MAP_RAM_REGS_H_

enum MAP_RAM {
	// Режим работы МАП см. enum MAP_MODE
	MAP_MODE	= 0x000,
	//  Показывает состояние заряда в котором находится МАП.
	MAP_StatusCH	= 0x002,
	// Была ли запись в epprom - нулить после чтения.
	MAP_put_eeprom	= 0x003,

	// АКБ
	// Состояние напряжения на АКБ
	MAP_StateUAcc	= 0x004,
	// Напряжение АКБ
	MAP_UAcc_med_VH = 0x005,
	MAP_UAcc_med_VL = 0x006,
	// Current high pression
	MAP_IAcc_med_A_L = 0x032,
	MAP_IAcc_med_A_H = 0x033,

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
	
	// BMS related
	// array of 32 elements each
	MAP_UakbCell_L	= 0x080,
	MAP_UakbCell_H	= 0x081,
	MAP_TakbCell	= 0x0C0,
	MAP_Q_Cell	= 0x0E0,
};

enum MAP_MODE {
	MAP_MODE_OFF	= 0, // off + nopower
	MAP_MODE_OFF_NET= 1, // off + power
	MAP_MODE_ON_GEN	= 2, // on + generation
	MAP_MODE_ON_NET	= 3, // on + power + nogeneration
	MAP_MODE_ONCHARGE=4, // on + charge
};

/*_StatusCh bits */
enum MAP_STATUSCH {
    ChargeIlow	= BIT(0),
    ChargeNeed	= BIT(1),
    ChargeEnd	= BIT(2),
    ChargeUmax	= BIT(3),
    /* Charing steps */
    ChargeBuffer = ChargeNeed|ChargeIlow,
    ChargeExtra = ChargeNeed|ChargeUmax,
    ChargeStep2 = ChargeNeed|ChargeEnd,
    ChargeStep1 = ChargeNeed,

};

#endif
