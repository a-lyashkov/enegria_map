_CFLAGS = -Wall -Werror -g3 -Iinclude/
_LIBOBJ = obj/serial.o obj/maplib_io.o obj/maplib_rom.o obj/maplib_ram.o
_LDFLAGS = -g3

all:
	gcc $(_CFLAGS) -c -o obj/serial.o lib/serial.c
	gcc $(_CFLAGS) -c -o obj/maplib_io.o lib/maplib_io.c
	gcc $(_CFLAGS) -c -o obj/maplib_rom.o lib/maplib_rom.c
	gcc $(_CFLAGS) -c -o obj/maplib_ram.o lib/maplib_ram.c
	gcc $(_CFLAGS) -c -o obj/hw_info.o src/hw_info.c
	gcc $(_CFLAGS) -c -o obj/in_info.o src/in_info.c
	gcc $(_LDFLAGS) -o hw_info obj/hw_info.o $(_LIBOBJ)
	gcc $(_LDFLAGS) -o in_info obj/in_info.o $(_LIBOBJ)

clean:
	rm -f obj/*
	rm -f hw_info in_info