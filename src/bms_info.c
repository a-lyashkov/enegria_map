#include <stdio.h>
#include <stdlib.h>

#include "serial.h"
#include "map_sin.h"

static char *serial_port="/dev/ttyUSB0";
const unsigned int serial_speed = 19200;

int main(int argc, char *argv[])
{
	int port;
	struct map_bms_info info;
	unsigned int i;

	port = port_open(serial_port, serial_speed);
	if (port < 0) {
		printf("error port open %d\n", port);
		return 1;
	}

	printf("port open OK, start process\n");

	if (map_bms_info(port, &info)<0) {
		printf("RAM isn't readed correctly ?\n");
		exit(3);
	}

	printf("BMS configuration: %u BMS in system\n", info.nbms);

	for(i=0;i<info.nbms;i++) {
	    printf("BMS %u\n", i);
	    printf("\tUAC %.2f\n", (float)info.bms[i].uac/100);
	    printf("\tI: %.2f\n", (float)info.bms[i].i/100);
	    if (info.bms[i].temp != MAP_BMS_TEMP_UNKNOW)
		    printf("\ttemp: %u\n", info.bms[i].temp);
	    printf("\tStatus: %s\n", info.bms[i].error ? "Error" : "OK");
	}

	return 0;
}
