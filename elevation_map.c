/* Create an elevation map using OpenSimplexNoise */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include "simplex_noise.h"

int main(int argc, char *argv[]) {
	int elevation_min, elevation_max, html_output;
	unsigned long height, width, map_size, y;
	double *map, map_min, map_max;
	int64_t seed;
	if (argc != 1 || scanf("%lu%lu%d%d%"SCNi64"%d", &height, &width, &elevation_min, &elevation_max, &seed, &html_output) != 6 || height < 2 || width < 2 || elevation_min > elevation_max) {
		fprintf(stderr, "%s: parameters expected on standard input: height width elevation_min elevation_max seed html_output\nheight > 1\nwidth > 1\nelevation_min <= elevation_max\n", argv[0]);
		fflush(stderr);
		return EXIT_FAILURE;
	}
	if (height > ULONG_MAX/width) {
		fprintf(stderr, "%s: map is too large\n", argv[0]);
		fflush(stderr);
		return EXIT_FAILURE;
	}
	map_size = height*width;
	map = malloc(sizeof(double)*map_size);
	if (!map) {
		fprintf(stderr, "%s: could not allocate memory for map\n", argv[0]);
		fflush(stderr);
		return EXIT_FAILURE;
	}
	simplex_noise_init(seed);
	map_min = 1;
	map_max = -1;
	for (y = 0; y < height; y++) {
		unsigned long x;
		double ny = y*2.0/height-1;
		for (x = 0; x < width; x++) {
			double nx = x*2.0/width-1;
			map[y*width+x] = simplex_noise(nx, ny)+0.5*simplex_noise(2*nx, 2*ny)+0.25*simplex_noise(4*nx, 4*ny);
			if (map[y*width+x] < map_min) {
				map_min = map[y*width+x];
			}
			if (map[y*width+x] > map_max) {
				map_max = map[y*width+x];
			}
		}
	}
	for (y = 0; y < height; y++) {
		unsigned long x;
		for (x = 0; x < width; x++) {
			map[y*width+x] = (map[y*width+x]-map_min)*(elevation_max-elevation_min)/(map_max-map_min)+elevation_min;
		}
	}
	if (html_output) {
		puts("<!DOCTYPE HTML>");
		puts("<HTML DIR=\"ltr\" LANG=\"en\">");
		puts("<HEAD>");
		puts("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; CHARSET=utf-8\">");
		puts("<TITLE>Elevation Map</TITLE>");
		puts("<STYLE TYPE=\"text/css\">");
		puts("BODY { font-family: Courier; }");
		puts("H1 { text-align: center; }");
		puts("TABLE { border-collapse: collapse; }");
		puts("TD { border: none; height: 4px; width: 4px; }");
		puts("</STYLE>");
		puts("</HEAD>");
		puts("<BODY>");
		puts("<H1>Elevation Map</H1>");
		puts("<TABLE>");
		printf("<CAPTION>Height %lu<BR>\nWidth %lu<BR>\nElevation Min. %d<BR>\nElevation Max. %d<BR>\nSeed %"PRIi64"</CAPTION>\n", height, width, elevation_min, elevation_max, seed);
		for (y = 0; y < height; y++) {
			unsigned long x;
			puts("<TR>");
			for (x = 0; x < width; x++) {
				int red = (int)((map[y*width+x]-elevation_min)/(elevation_max-elevation_min)*255), green = 255-red;
				printf("<TD STYLE=\"background-color: #%02x%02x00;\"></TD>\n", red, green);
			}
			puts("</TR>");
		}
		puts("</TABLE>");
		puts("</BODY>");
		puts("</HTML>");
	}
	else {
		printf("%lu %lu\n", height, width);
		for (y = 0; y < height; y++) {
			unsigned long x;
			printf("%d", (int)map[y*width]);
			for (x = 1; x < width; x++) {
				printf(" %d", (int)map[y*width+x]);
			}
			puts("");
		}
	}
	fflush(stdout);
	free(map);
	return EXIT_SUCCESS;
}
