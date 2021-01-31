/*
 **@file
 *
 *@brief main code file for the modulation functions: on and off keying (ook),
 *amplitude shift keying (ask), binary phase shift keying (bpsk), and
 *quadrate phase keying (qpsk)
 *
 *@author ECE486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date March 2020.
 *
 *
 */
 /*****************************************************************

  file: keying.c
  Description:routines to implement four differnt types of modulation.


  Interface:
      keying_int() initializes the variables used for each of
			the function calls

   Function Prototypes and parameters:

         #include "nco.h"
        BP_T *keying_init(int *bit_pattern, int pattern_length, int bit_rate);
            Inputs:
                 bit_pattern    Initial pointer to check the bit
                 pattern_length Intial length of each segment
								 bit_rate Intial bit change
            Returned:
                 The function returns a pointer to a "BP_T" data type
                 (which is defined in "nco.h")

         void ook(NCO_T *s, BP_T *b);
            Inputs:
               s         pointer to initialized NCO_T
               b        pointer to inialized BP_T
            Returned:
               On return, y[i] (for i=0,1,...n_samples-1) contains the the next
               n_samples values of the NCO output sequence.  Subsequent calls to
               nco_get_samples() continues to return NCO output samples with no
               phase discontinuities from one call to the next. The BP_T returns the
							 length, bitanry pattern and rate
               Example:
                 NCO_T *s;
                 float y1[15],y2[5];
                 s=init_nco(0.11, 0.0);       // set up NCO for .11 cycles/sample
                 nco_get_samples(s, y1, 15);  // Generate samples 0,1, ..., 14 of y(n)
                 nco_get_samples(s, y2, 5);   // Generate samples 15,16,17,18,19 of y(n)

         void ask(NCO_T *s, BP_T *b);
           Inputs:
              s         pointer to NCO_T
              b        pointer to inialized BP_T
           Returned:
					 On return, y[i] (for i=0,1,...n_samples-1) contains the the next
					 n_samples values of the NCO output sequence.  Subsequent calls to
					 nco_get_samples() continues to return NCO output samples with no
					 phase discontinuities from one call to the next. The BP_T returns the
					 length, bitanry pattern and rate

         void bpsk(NCO_T *s, BP_T *b);
				 Inputs:
						s         pointer to NCO_T
						b        pointer to inialized BP_T
				 Returned:
				 On return, y[i] (for i=0,1,...n_samples-1) contains the the next
				 n_samples values of the NCO output sequence.  Subsequent calls to
				 nco_get_samples() continues to return NCO output samples with no
				 phase discontinuities from one call to the next. The BP_T returns the
				 length, bitanry pattern and rate

         void qpsk(NCO_T *s1, NCO_T *s2, BP_T *b, float *y, float*y1);
				 Inputs:
					 s1         pointer to NCO_T
					 s2        pointer to NCO_T
					 b        pointer to inialized BP_T
					 y				float used to get samples of coseine and sine term
					 y1      float used to get samples of coseine and sine term
				Returned:
				On return, y[i] (for i=0,1,...n_samples-1) contains the the next
				n_samples values of the NCO output sequence.  Subsequent calls to
				nco_get_samples() continues to return NCO output samples with no
				phase discontinuities from one call to the next. The BP_T returns the
				length, bitanry pattern and rate. y and y1 returns the sine and cosine
				output

 *******************************************************************/
 // C code for keying functions

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "ece486_nco.h"
#include "keying.h"

BP_T *keying_init(int *bit_pattern, int pattern_length, int bit_rate) {
	// Allocate memory for keying struct
	BP_T *b = (BP_T *)malloc(sizeof(BP_T));

	// Initialize values
	b->pattern_length = pattern_length; // Set length of the bit pattern
	b->bit_rate = bit_rate; // Set bit rate (not used)
	b->pos = 0; // Position within the bit pattern
	b->block_count = 0; // Number of blocks per encoded bit

	// Allocate memory for the bit pattern
	b->bit_pattern = (int *)calloc(b->pattern_length, sizeof(int));

	// Set the bit pattern in the struct
	int i;
	for (i = 0; i < b->pattern_length; i++) {
		b->bit_pattern[i] = bit_pattern[i];
	}

	// Return the initialized struct
	return b;
}

void ook(NCO_T *s, BP_T *b) {
	// On-Off Keying
	// Every 200 samples is a bit

	// Get bit in current position /2 since 2 blocks per bit
	// Either 0 or 1 so signal or no signal encoding
	s->amp = 0.9*b->bit_pattern[b->pos/2];

	// Increment bit position
	b->pos++;

	// Make sure pos is within pattern length
	if (b->pos >= 2*b->pattern_length) {
		b->pos = 0;
	}


}


void ask(NCO_T *s, BP_T *b) {
	// Amplitude Shift Keying

	// Get bit in current position/2 since 2 blocks per bit
	if (b->bit_pattern[b->pos/2]) {
		s->amp = 0.9; // Encoded 1 as almost full amplitude to be < 1
	} else {
		s->amp = 0.45; //1/2 max amplitude to encode a zero
	}

	// Increment bit position
	b->pos++;

	// Make sure pos is within pattern limit
	if (b->pos >= 2*b->pattern_length) {
		b->pos = 0;
	}
}

void bpsk(NCO_T *s, BP_T *b) {
	// Binary Phase Shift Keying

	// Make sure amplitude is set such that the DAC doesnt overflow
	s->amp = 0.9;

	// Increment block count on each call
	b->block_count++;

	// Increment Bit position every 2 blocks = 1ms
	if (b->block_count>=2) {
		b->pos++;

		// Reset the number of blocks for next bit
		b->block_count = 0;


		// Make sure pos within pattern limit
		if (b->pos > b->pattern_length) {
			b->pos = 0;

		}

		// Set the phase for the current block based on the bit value
		if (b->bit_pattern[b->pos]) {
			nco_set_phase(s, 0.0); //0 degrees phase shift
		} else {
			nco_set_phase(s, M_PI); //180 degrees phase shift
		}
	}


}


void qpsk(NCO_T *s1, NCO_T *s2, BP_T *b, float *y, float *y2) {
	// Quadrature Phase Shift Keying
	// 4 * 100 now because only change every 2ms not 1ms

	// Increment block count
	b->block_count++;

	// Increment Bit position based on 4 blocks being 2ms
	if (b->block_count >= 4) {
		b->pos+=2;

		// Make sure pos is within pattern limit
		if ((b->pos+1) > b->pattern_length) {
			b->pos = 0;
			b->block_count = 0;
		}

		// Reset the block count
		b->block_count = 0;

	}

	// Set the amplitude for the cosine term
	if (b->bit_pattern[b->pos]) {
		s1->amp = 0.5;
	} else {
		s1->amp = -0.5;
	}

	// Set the amplitude for the sine term either + or - which corresponds
	// to 0 or 180 degrees phase shift
	if (b->bit_pattern[b->pos+1]) {
		s2->amp = -.5;
	} else {
		s2->amp = 0.5;
	}

	// Get the samples for cosine term
	nco_get_samples(s1, y, 100);

	// Get the samples for the sine term
	nco_get_samples(s2, y2, 100);

	// Set up output 2 to be bit sequence
	float outBit = 0.3 * (b->bit_pattern[b->pos]*2+b->bit_pattern[b->pos+1]);

	// Sum the two signals and send to output
	int i;
	for (i = 0; i < 100; i++) {
		y[i] += y2[i]; // Sum the cosine and sine terms
		y[i] *= 0.9; // Scale to 90% so that the DAC doesnt overflow
		y2[i] = outBit; //Make it clear what sequence is encoded
	}








}
