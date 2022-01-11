#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "serial.h"

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


    close(port);
}
