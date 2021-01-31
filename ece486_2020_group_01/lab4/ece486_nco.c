/*
 **@file
 *
 *@brief main file for the Numerically controlled ocisllator (NCO)
 *
 *@author ECE486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date March 2020.
 *
 *
 */

/*****************************************************************


 file: ece486_nco.c
 Description:  Subroutines to implement numerically controlled oscillators
        Multiple subroutine calls are used to create sample sequences
        for sinusoidal functions with programmable frequency and phase.
            y(n) = cos(2 pi f0 n + theta)


 Interface:
     init_nco() is called once (for each oscillator), and is used to create
                any required oscillator structures.  This routine handles all
                array initialization, allocates any required memory,
                and perform any other required initialization.
     nco_set_frequency() is used to change the frequency of the nco
     nco_set_phase() is used to change the phase of the nco
     nco_get_samples() return a block of nco output samples.
     destroy_nco() is called once at the end of the program, and is used
                  to de-allocate any memory associated with an nco.

  Function Prototypes and parameters:

        #include "nco.h"
        NCO_T *init_nco(float f0, float theta);
           Inputs:
                f0     Initial NCO discrete-time frequency for the output sample
                       sequence (in cycles/sample)
                theta  Inital Phase of the output sample sequence (radians)
           Returned:
                The function returns a pointer to a "NCO_T" data type
                (which is defined in "nco.h")

        void nco_get_samples(NCO_T *s, float *y, int n_samples);
           Inputs:
              s         pointer to initialized NCO_T
              y         Caller-supplied array to be replaced by NCO output samples
              n_samples Number of NCO output samples to generate.
           Returned:
              On return, y[i] (for i=0,1,...n_samples-1) contains the the next
              n_samples values of the NCO output sequence.  Subsequent calls to
              nco_get_samples() continues to return NCO output samples with no
              phase discontinuities from one call to the next.
              Example:
                NCO_T *s;
                float y1[15],y2[5];
                s=init_nco(0.11, 0.0);       // set up NCO for .11 cycles/sample
                nco_get_samples(s, y1, 15);  // Generate samples 0,1, ..., 14 of y(n)
                nco_get_samples(s, y2, 5);   // Generate samples 15,16,17,18,19 of y(n)

        void nco_set_frequency(NCO_T *s, float f_new);
          Inputs:
             s         pointer to NCO_T
             f_new     New NCO frequency (in cycles/sample).
          Returned:
             The NCO_T structure s is modified so that subsequent calls to nco_get_samples()
             will operate at frequency f_new (without loss of phase continuity).

        void nco_set_phase(NCO_T *s, float theta);
          Inputs:
             s         pointer to NCO_T
             theta     New NCO phase.
          Returned:
             The NCO_T structure s is modified so that subsequent calls to nco_get_samples()
             will operate with the phase shift given by theta (in radians)

        void destroy_nco(NCO_T *s);
           Inputs:
                s       pointer to NCO_T, as provided by init_nco()
           Returned: Any resources associated with the nco "s" are released.


*******************************************************************/
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include "ece486_nco.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "arm_math.h"
#include <stdint.h>

NCO_T *init_nco(float f0, float theta){
	// Allocate memory for  NCO_T struct
	NCO_T *s = (NCO_T *)malloc(sizeof(NCO_T));

	// Check for allocation errors
	if (s == NULL) {
		printf("Failed to allocate memory for NCO_T *s\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
		return NULL;
	}

	// Set attributes of NCO_T struct based on inputs
	s->f0_step = f0*pow(2, 32); // Caculate index step based on target freq
	s->theta_step = theta*pow(2,32)/(2*M_PI); // Step size for phase shift
	s->ctrlWord = s->theta_step; // Offset index in array by phase shift
	s->amp = 0.9; // Amplitude < 1 to not overflow DAC
	s->theta = theta; // Keep track of theta value
	
	// Allocate memory for lookup table
	s->lookup = (float *)calloc(512, sizeof(float));

	// Check for errors
	if (s->lookup == NULL) {
		printf("Failed to allocate memory for lookup table\n");
		flagerror(MEMORY_ALLOCATION_ERROR);
		while(1);
		return NULL;
	}

	// Generate lookup table values for 1 cycle of cosine at freq f0
	int k;
	for (k = 0; k < 512; k++) {
		s->lookup[k] = cos(2*M_PI*k/512);
	}

	// Return created NCO_T struct
	return s;

}

// Return n samples of the NCO function from lookup table
void nco_get_samples(NCO_T *s, float *y, int n_samples){

	int k;
	for (k=0; k < n_samples; k++) {
		// Phase accumulator, increment by frequency step size
		s->ctrlWord += s->f0_step;

		// Get sample value based on lookup table and scaled by amp
		y[k] = s->amp * s->lookup[(s->ctrlWord>>23)&0x1FF];
	}
}

// Set the new frequency of NCO by recalculating the new frequency step size
void nco_set_frequency(NCO_T *s, float f_new){
	// Set the f0 val in NCO_T struct to f_new
	s->f0_step = f_new*TWO32F;
}

// Set a new phase for the NCO by offsetting the current index
void nco_set_phase(NCO_T *s, float theta){
	// Calculate a new theta_step value for index offset
	s->theta_step = theta*TWO32F/(2*M_PI);

	// Track the floating point value of the phase shift
	s->theta = theta;

	// Apply index offset due to phase shift to the index into the table
	s->ctrlWord += s->theta_step;
}


void destroy_nco(NCO_T *s){
	// Free memory inside NCO_T struct
	if (s->lookup != NULL) {
		free(s->lookup);
	}

	// Free the NCO_T struct
	if (s != NULL) {
		free(s);
	}
}
