ELEVATION_MAP_C_FLAGS=-std=c99 -g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

elevation_map_debug: simplex_noise_debug.o elevation_map_debug.o
	gcc -g -o elevation_map_debug simplex_noise_debug.o elevation_map_debug.o

simplex_noise_debug.o: simplex_noise.c simplex_noise.h elevation_map_debug.make
	gcc -c ${ELEVATION_MAP_C_FLAGS} -o simplex_noise_debug.o simplex_noise.c

elevation_map_debug.o: elevation_map.c simplex_noise.h elevation_map_debug.make
	gcc -c ${ELEVATION_MAP_C_FLAGS} -o elevation_map_debug.o elevation_map.c

clean:
	rm -f elevation_map_debug simplex_noise_debug.o elevation_map_debug.o
