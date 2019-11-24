/* C port of KdotJPG's OpenSimplexNoise 2D function */
/* Original code https://gist.github.com/KdotJPG/b1270127455a94ac5d19 */

#include <inttypes.h>
#include <math.h>

#include "simplex_noise.h"
static int fast_floor(double);
static double extrapolate(int, int, double, double);

int32_t perm[256];

void simplex_noise_init(int64_t seed) {
	int i;
	int32_t source[256];
	static int64_t lcg_mul_constant = INT64_C(6364136223846793005), lcg_add_constant = INT64_C(1442695040888963407);
	for (i = 0; i < 256; i++) {
		source[i] = i;
	}
	seed = seed*lcg_mul_constant+lcg_add_constant;
	seed = seed*lcg_mul_constant+lcg_add_constant;
	seed = seed*lcg_mul_constant+lcg_add_constant;
	for (i = 256; i > 0; i--) {
		int r;
		seed = seed*lcg_mul_constant+lcg_add_constant;
		r = (int)((seed+31)%i);
		if (r < 0) {
			r += i;
		}
		perm[i-1] = source[r];
		source[r] = source[i-1];
	}
}

double simplex_noise(double x, double y) {
	double stretch_constant = (1/sqrt(3)-1)/2, stretch_offset = (x+y)*stretch_constant, xs = x+stretch_offset, ys = y+stretch_offset;
	int xsb = fast_floor(xs), ysb = fast_floor(ys), xsv_ext, ysv_ext;
	double squish_constant = (sqrt(3)-1)/2, squish_offset = (xsb+ysb)*squish_constant, xb = xsb+squish_offset, yb = ysb+squish_offset, xins = xs-xsb, yins = ys-ysb, in_sum = xins+yins, dx0 = x-xb, dy0 = y-yb, value = 0, dx1, dy1, attn1, dx2, dy2, attn2, dx_ext, dy_ext, attn0, attn_ext, norm_constant = 47;
	dx1 = dx0-1-squish_constant;
	dy1 = dy0-squish_constant;
	attn1 = 2-dx1*dx1-dy1*dy1;
	if (attn1 > 0) {
		attn1 *= attn1;
		value += attn1*attn1*extrapolate(xsb+1, ysb, dx1, dy1);
	}
	dx2 = dx0-squish_constant;
	dy2 = dy0-1-squish_constant;
	attn2 = 2-dx2*dx2-dy2*dy2;
	if (attn2 > 0) {
		attn2 *= attn2;
		value += attn2*attn2*extrapolate(xsb, ysb+1, dx2, dy2);
	}
	if (in_sum <= 1) {
		double zins = 1-in_sum;
		if (zins > xins || zins > yins) {
			if (xins > yins) {
				xsv_ext = xsb+1;
				ysv_ext = ysb-1;
				dx_ext = dx0-1;
				dy_ext = dy0+1;
			}
			else {
				xsv_ext = xsb-1;
				ysv_ext = ysb+1;
				dx_ext = dx0+1;
				dy_ext = dy0-1;
			}
		}
		else {
			xsv_ext = xsb+1;
			ysv_ext = ysb+1;
			dx_ext = dx0-1-2*squish_constant;
			dy_ext = dy0-1-2*squish_constant;
		}
	}
	else {
		double zins = 2-in_sum;
		if (zins < xins || zins < yins) {
			if (xins > yins) {
				xsv_ext = xsb+2;
				ysv_ext = ysb;
				dx_ext = dx0-2-2*squish_constant;
				dy_ext = dy0-2*squish_constant;
			}
			else {
				xsv_ext = xsb;
				ysv_ext = ysb+2;
				dx_ext = dx0-2*squish_constant;
				dy_ext = dy0-2-2*squish_constant;
			}
		}
		else {
			xsv_ext = xsb;
			ysv_ext = ysb;
			dx_ext = dx0;
			dy_ext = dy0;
		}
		xsb++;
		ysb++;
		dx0 -= 1+2*squish_constant;
		dy0 -= 1+2*squish_constant;
	}
	attn0 = 2-dx0*dx0-dy0*dy0;
	if (attn0 > 0) {
		attn0 *= attn0;
		value += attn0*attn0*extrapolate(xsb, ysb, dx0, dy0);
	}
	attn_ext = 2-dx_ext*dx_ext-dy_ext*dy_ext;
	if (attn_ext > 0) {
		attn_ext *= attn_ext;
		value += attn_ext*attn_ext*extrapolate(xsv_ext, ysv_ext, dx_ext, dy_ext);
	}
	return value/norm_constant;
}

static int fast_floor(double x) {
	int xi = (int)x;
	return x < xi ? xi-1:xi;
}

static double extrapolate(int xsb, int ysb, double dx, double dy) {
	static int8_t gradients[] = { 5, 2, 2, 5, -5, 2, -2, 5, 5, -2, 2, -5, -5, -2, -2, -5 };
	int index = perm[(perm[xsb & 0xFF]+ysb) & 0xFF] & 0x0E;
	return gradients[index]*dx+gradients[index+1]*dy;
}
