#ifndef _MAP_SIN_BAT_H_
#define _MAP_SIN_BAT_H_

/*
 * Information about battery
 */

#include <stdint.h>

struct map_bat_info {
	uint16_t	uac;  // V * 10
	uint8_t		i;    // A * 16 
	uint32_t	pow;  // power in W
	unsigned long	charge:1, // charge started
			buffer:1, // buffer charge
			step1:1,  // step1
			step2:1,  // step2
			extra:1;  // dozarad	
};


/*
 return information about battry
 return error if ram/epprom don't read before
 */
extern int map_bat_info(int fd, struct map_bat_info *info);


#endif
