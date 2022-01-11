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
    struct map_hw_info info;

    port = port_open(serial_port, serial_speed);
    if (port < 0) {
	    printf("error port open %d\n", port);
	    return 1;
    }

    printf("port open OK, start process\n");

    if (map_epprom_read(port) < 0) {
	    printf("error reading from port\n");
	    goto end;
    }

    map_hw_info(&info);

    printf("Map INFO\n");
    printf("\tdevice serial %#0x\n", info.serial);
    printf("\tdevice HW revision %d\n", info.hw_ver);
    printf("\tdevice FW revission %d.%d\n", 
	MAP_FW_MAJOR(info.fw_ver), MAP_FW_MINOR(info.fw_ver));
    printf("\tdevice power %s\n", map_pow_str(info.pow));
    printf("\tdevice battery %s\n", map_bat_str(info.bat));


end:
    close(port);
}
