#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "map_int.h"
#include "map_ram_regs.h"

#include "map_sin_rom.h"
#include "map_sin_in.h"
#include "map_sin_bat.h"
#include "map_sin_bms.h"

int map_in_info(int fd, struct map_in_info *info)
{
	if (!ram_valid) {
		if (map_ram_read(fd) < 0)
			return -EIO;
	}

	if (!ram_valid)
		return -EINVAL;

	info->uac	= map_ram[MAP_UNET] ? map_ram[MAP_UNET] + 100 : 0;
	info->i		= map_ram[MAP_INET];
	info->pow	= conv_ram_u16(MAP_PNET_H, MAP_PNET_L) * 100;
	info->link	= 0;

	return 0;
}

int map_bat_info(int fd, struct map_bat_info *info)
{
	if (!ram_valid) {
		if (map_ram_read(fd) < 0)
			return -EIO;
	}

	if (!ram_valid)
		return -EINVAL;

	info->uac	= conv_ram_u16(MAP_UAcc_med_VH,MAP_UAcc_med_VL);
	info->i		= conv_ram_u16(MAP_IAcc_med_A_H, MAP_IAcc_med_A_L);
	info->pow	= 0;
	info->charge	= map_ram[MAP_MODE] == MAP_MODE_ONCHARGE;
	info->buffer	= (map_ram[MAP_StatusCH] & ChargeBuffer) == ChargeBuffer;
	info->step1	= map_ram[MAP_StatusCH] == ChargeStep1;
	info->step2	= (map_ram[MAP_StatusCH] & ChargeStep2) == ChargeStep2;
	info->extra	= (map_ram[MAP_StatusCH] & ChargeExtra) == ChargeExtra;

	return 0;
}


int map_bms_info(int fd, struct map_bms_info *info)
{
	struct map_hw_info hw;
	unsigned int nbms;
	unsigned int i;
	int rc;
	
	rc = map_hw_info(fd, &hw);
	if (rc < 0)
		return rc;


	if (!ram_valid) {
		if (map_ram_read(fd) < 0)
			return -EIO;
	}

	if (!ram_valid)
		return -EINVAL;

	// XXX MAP have assumption - 12V - 4BMS, 96V - 16BMS.
	switch(hw.bat) {
	case MAP_BAT_12:
		nbms = 4;
		break;
	case MAP_BAT_24:
		nbms = 8;
		break;
	case MAP_BAT_48:
		nbms = 16;
		break;
	case MAP_BAT_96:
		nbms = 32;
	default:
		return -EPROTO;
	}


	info->nbms = nbms;
	memset(info->bms, 0, sizeof(info->bms));
	for (i=0; i<nbms; i++) {
	    uint16_t uac;

	    uac = conv_ram_u16(MAP_UakbCell_H+i*2, MAP_UakbCell_L);
	    info->bms[i].uac = uac & 0x7FFF;
	    info->bms[i].error = uac & 0x8000;
	    info->bms[i].temp = map_ram[MAP_TakbCell+i] != 255 ? 
			    map_ram[MAP_TakbCell+i]-50 : 
			    MAP_BMS_TEMP_UNKNOW;
	    info->bms[i].i = ((uint32_t)map_ram[MAP_Q_Cell+i] * info->bms[i].uac)/100;
	}

	return 0;
}