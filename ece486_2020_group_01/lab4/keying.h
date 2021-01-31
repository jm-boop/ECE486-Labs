/*
 **@file
 *
 *@brief header file for the modulation functions: on and off keying (ook),
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

  file: keying.h
  Description:  Subroutines to implement four differnt types of modulation.


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


#ifndef KEYING_H
#define KEYING_H

typedef struct {
	int *bit_pattern; // Bit pattern to encode
	int pos; // Track position within the bit pattern
	int bit_rate; // How many bits per second (not used)
	int pattern_length; // Length of the bit pattern for addressing
	int block_count; // Keep track of number of blocks per bit
} BP_T;

// Function for initializing the keying struct
BP_T *keying_init(int *bit_pattern, int pattern_length, int bit_rate);

// Function to produce On Off Keying (OOK)
void ook(NCO_T *s, BP_T *b);

// Function to produce Amplitude Shift Keying (ASK)
void ask(NCO_T *s, BP_T *b);

// Function to produce Binary Phase Shift Keying (BPSK)
void bpsk(NCO_T *s, BP_T *b);

// Function to produce Quadrature Phase Shift Keying (QPSK)
void qpsk(NCO_T *s1, NCO_T *s2, BP_T *b, float *y, float*y1);

#endif // KEYING_H
