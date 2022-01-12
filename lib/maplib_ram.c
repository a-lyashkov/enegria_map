#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "map_int.h"
#include "map_ram_regs.h"
#include "map_sin_in.h"

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
	info->pow	= ((map_ram[MAP_PNET_H] << 8) + map_ram[MAP_PNET_L]) * 100;
	info->link	= 0;

	return 0;
}
