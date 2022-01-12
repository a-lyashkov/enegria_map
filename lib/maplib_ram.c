#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "map_int.h"
#include "map_ram_regs.h"
#include "map_sin_in.h"
#include "map_sin_bat.h"

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
