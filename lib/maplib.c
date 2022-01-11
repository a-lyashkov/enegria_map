#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "map_sin.h"
#include "map_regs.h"

static uint8_t *epprom;
static bool epprom_valid;

int map_epprom_read(int fd)
{
	epprom = NULL;
	epprom_valid = true;

	return 0;
}