ELEVATION_MAP_C_FLAGS=-std=c99 -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

elevation_map: simplex_noise.o elevation_map.o
	gcc -o elevation_map simplex_noise.o elevation_map.o -lm

simplex_noise.o: simplex_noise.c simplex_noise.h elevation_map.make
	gcc -c ${ELEVATION_MAP_C_FLAGS} -o simplex_noise.o simplex_noise.c

elevation_map.o: elevation_map.c simplex_noise.h elevation_map.make
	gcc -c ${ELEVATION_MAP_C_FLAGS} -o elevation_map.o elevation_map.c

clean:
	rm -f elevation_map simplex_noise.o elevation_map.o
