#ifndef _MAP_SIN_BMS_H_
#define _MAP_SIN_BMS_H_

/*
 * Information about bms connected to MAP
 */

#include <stdint.h>
#include <stdbool.h>

#define MAP_BMS_MAX	32
#define MAP_BMS_TEMP_UNKNOW 255

struct map_bms {
	uint16_t	uac; // *100
	uint16_t	i;   //
	uint8_t		temp;
	bool		error;
};

struct map_bms_info {
	unsigned int	nbms; // number BMS connected
	struct map_bms	bms[MAP_BMS_MAX];
};


/*
 return information about battry
 return error if ram/epprom don't read before
 */
extern int map_bms_info(int fd, struct map_bms_info *info);


#endif
