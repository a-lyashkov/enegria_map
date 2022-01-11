#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int port_open(const char *port, unsigned int speed)
{
    int fd;
    struct termios settings;
    int s_speed;

    fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
	return -errno;

    tcgetattr(fd, &settings);
    cfmakeraw (&settings);

    /* XXX replace with real settings */

    /* Setting the Baud rate */
    /* todo autodetect */
    switch(speed) {
    case 9600:
	s_speed = B9600;
	break;
    case 19200:
	s_speed = B19200;
	break;
    default:
	return -EINVAL;
    }

    cfsetispeed(&settings,s_speed); // in
    cfsetospeed(&settings,s_speed); // out

    /* 8N1 Mode */
    settings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
    settings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    settings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
    settings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

    settings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    settings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    /* Setting Time outs */
    settings.c_cc[VMIN] = 1; /* Read at least 1 character */
    settings.c_cc[VTIME] = 50; /* 5s MAP timeout 1/10s  */

    if ((tcsetattr(fd,TCSANOW,&settings)) != 0)
	return -errno;

    tcflush(fd, TCIFLUSH);
    tcflush(fd, TCOFLUSH);

    return fd;
}
