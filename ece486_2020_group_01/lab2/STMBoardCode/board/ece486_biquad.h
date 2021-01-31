/*
* ece486_biquad.h
*
*/

#ifndef ECE486_BIQUAD_H
#define ECE486_BIQUAD_H

typedef struct biquad{

	int sections;
	float g;
	float *biquad_coefs;
	int blocksize;
	float *v1M;
	float *v2M;

} BIQUAD_T;


BIQUAD_T *init_biquad(int sections, float g, float *biquad_coefs, int blocksize);
void calc_biquad(BIQUAD_T *s, float *x, float *y);
void destroy_biquad(BIQUAD_T *s);

#endif // ECE486_BIQUAD_H
