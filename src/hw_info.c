#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "serial.h"
#include "map_sin.h"

static char *serial_port="/dev/ttyUSB0";
const unsigned int serial_speed = 19200;

int main(int argc, char *argv[])
{
    int port;

    port = port_open(serial_port, serial_speed);
    if (port < 0) {
	    printf("error port open %d\n", port);
	    return 1;
    }

    printf("port open OK, start process\n");

    if (map_epprom_read(port) < 0)
	    printf("error reading from port\n"); 

    close(port);
}
