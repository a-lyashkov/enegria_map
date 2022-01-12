#ifndef _MAP_SIN_IN_H_
#define _MAP_SIN_IN_H_

/*
 * Information about input power
 */

#include <stdint.h>
#include <stdbool.h>

struct map_in_info {
	uint16_t	uac;  // V
	uint8_t		i;    // A
	uint32_t	pow;  // power in W
	uint8_t		link; // input link
};


/*
 return information about input link.
 return error if epprom don't read before
 */
extern int map_in_info(int fd, struct map_in_info *info);


#endif
