#include <stdio.h>
#include <stdlib.h>

#include "serial.h"
#include "map_sin.h"

static char *serial_port="/dev/ttyUSB0";
const unsigned int serial_speed = 19200;

int main(int argc, char *argv[])
{
	int port;
	struct map_in_info info;

	port = port_open(serial_port, serial_speed);
	if (port < 0) {
		printf("error port open %d\n", port);
		return 1;
	}

	printf("port open OK, start process\n");

	if (map_in_info(port, &info)<0) {
		printf("RAM isn't readed correctly ?\n");
		exit(3);
	}

	printf("MAP current input\n");
	printf("\tInput Voltage %u (V)\n", info.uac);
	printf("\tInput current %u (A)\n", info.i);
	printf("\tInput power %u (W)\n", info.pow);


	return 0;
}
