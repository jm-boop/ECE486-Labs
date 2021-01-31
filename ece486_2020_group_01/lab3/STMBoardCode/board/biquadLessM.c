/*
*ece486_biquad.c
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "ece486_biquad.h"
#include <string.h>

BIQUAD_T *init_biquad(int sections, float g, float *biquad_coefs, int blocksize)
{
    BIQUAD_T *s = (BIQUAD_T *)malloc(sizeof(BIQUAD_T)); // Create biquad struct

    // Fill variables within pointer to struct
    s->sections = sections;
    s->g = g;
    s->biquad_coefs = biquad_coefs;
    s->blocksize = blocksize;
    //s->v1 = (float *)calloc(blocksize, sizeof(float));
    //s->v2 = (float *)calloc(blocksize, sizeof(float));
    // Less Memory
    s->v1M = (float *)calloc(2, sizeof(float));
    s->v2M = (float *)calloc(2, sizeof(float));


    return s; // Return biquad struct
}

void calc_biquad(BIQUAD_T *s, float *x, float *y)
{
    //Calculate outputs
    // n-1 will become zero
    // n will become 1
    int j;
    for (j = 0; j < s->sections; j++) {
	memset(s->v1M, 0, 2*sizeof(float));
	memset(s->v2M, 0, 2*sizeof(float));

	int n;
        for (n = 0; n < s->blocksize;n++) {
            ///index the mod wont work here because we need to replace old values
            y[n] = s->v1M[0] + s->biquad_coefs[(j*5)]*x[n];
            s->v1M[1] = s->v2M[0]-s->biquad_coefs[(j*5)+3]*y[n]+s->biquad_coefs[(j*5)+1]*x[n];
            s->v2M[1] = s->biquad_coefs[(j*5)+2]*x[n] - s->biquad_coefs[(j*5)+4] * y[n];
            x[n] = y[n];

            printf("The value ofV1M[0] is %f\n", s->v1M[0]);
            s->v1M[0]=s->v1M[1];

            s->v2M[0]=s->v2M[1];
        }
    }

}

void destroy_biquad(BIQUAD_T *s)
{
    free(s);
}
