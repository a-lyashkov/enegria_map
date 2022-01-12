#include <stdio.h>
#include <stdlib.h>

#include "serial.h"
#include "map_sin.h"

static char *serial_port="/dev/ttyUSB0";
const unsigned int serial_speed = 19200;

int main(int argc, char *argv[])
{
	int port;
	struct map_bat_info info;

	port = port_open(serial_port, serial_speed);
	if (port < 0) {
		printf("error port open %d\n", port);
		return 1;
	}

	printf("port open OK, start process\n");

	if (map_bat_info(port, &info)<0) {
		printf("RAM isn't readed correctly ?\n");
		exit(3);
	}

	printf("MAP battary state\n");
	printf("\tVoltage %.2f (V)\n", (float)info.uac/10);
	printf("\tcurrent %.2f (A)\n", (float)info.i / 16);
	printf("\tpower %u (W)\n", info.pow);
	if (info.charge) {
		printf("\tCharging state:\n");
		printf("\t\t");
		if (info.buffer)
			printf("buffer ");
		if (info.step1)
			printf("step1 ");
		if (info.step2)
			printf("step2 ");
		if (info.extra)
			printf("extra_charge ");
		printf("\n");
	} else
		printf("\tNot charging\n");
	printf("\tErrors:\n");
	printf("\tWarrings:\n");


	return 0;
}
