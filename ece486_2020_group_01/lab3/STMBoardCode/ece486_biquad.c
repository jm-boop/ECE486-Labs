/*
*@file
*
*@brief ECE 486 IRR filter code
*
*@author ECE486 Lab Group 1
*@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
*
*@date Febuary 2020
*
*This file contains the subroutine defintions necessary for doing
*the IIR filter calcutions on a block of samples.
*
*The init_biquad() function initializes a BIQUAD_T varaiable, and allocates
*memory. The function takes an integer 'sections', which is the number of
*biquad filter sections used in the transfer function calculations.
*The float 'g' is passed as the gain and multiplies every biquad section.
*v1 and v2 hold the coeffinences for the calcuations.
*The function returns a pointer to a Biquad_T struct, 's'.
*
*The destroy_biquad() function clears all the memory
*pointed to by 	BIQUAD_T *'s'.
*/

#include <stdio.h>
#include <stdlib.h>
#include "ece486_biquad.h"
/*!
 * @brief Initializes a BIQUAD_T structure.
 *
 * @returns A pointer to a structure of type BIQUAD_T is returned.
 */
BIQUAD_T *init_biquad(int sections, float g, float *biquad_coefs, int blocksize)
{
	BIQUAD_T *s = (BIQUAD_T *)malloc(sizeof(BIQUAD_T)); // Create biquad struct

	// Fill variables within pointer to struct
	s->sections = sections;
	s->g = g;
	s->biquad_coefs = biquad_coefs;
	s->blocksize = blocksize;
	s->v1 = (float *)calloc(sections, sizeof(float));
	s->v2 = (float *)calloc(sections, sizeof(float));

	return s; // Return biquad struct
}
/*!
 * @brief Performs IIR filter calculation on block of samples
 *
 * @returns the output array 
 */

void calc_biquad(BIQUAD_T *s, float *x, float *y)
{
	int j, n;
	//Iterate through each section and calculate output of the section
	for (j = 0; j < s->sections; j++) {

		//Calculate output sample by sample
		for (n = 0; n < s->blocksize; n++) {

			// Apply gain to the input of the first stage of the filter
			if (j == 0) {
				x[n] = s->g * x[n];
			}

			y[n] = s->v1[j] + s->biquad_coefs[(j*5)]*x[n];
			s->v1[j] = s->v2[j]-s->biquad_coefs[(j*5)+3]*y[n]+s->biquad_coefs[(j*5)+1]*x[n];
			s->v2[j] = s->biquad_coefs[(j*5)+2]*x[n] - s->biquad_coefs[(j*5)+4] * y[n];
			x[n] = y[n];
		}


	}

}

void destroy_biquad(BIQUAD_T *s)
{
	free(s->v1);
	free(s->v2);
	free(s);
}
