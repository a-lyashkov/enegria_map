_CFLAGS = -Wall -Werror -g3 -Iinclude/
_LIBOBJ = obj/serial.o obj/maplib.o
_LDFLAGS = -g3

all:
	gcc $(_CFLAGS) -c -o obj/serial.o lib/serial.c
	gcc $(_CFLAGS) -c -o obj/maplib.o lib/maplib.c
	gcc $(_CFLAGS) -c -o obj/hw_info.o src/hw_info.c
	gcc $(_LDFLAGS) -o hw_info obj/hw_info.o $(_LIBOBJ)

clean:
	rm -f obj/*
	rm -f hw_info