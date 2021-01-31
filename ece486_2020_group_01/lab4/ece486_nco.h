/*
 **@file
 *
 *@brief header file for the Numerically controlled ocisllator (NCO)
 *
 *@author ECE486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date March 2020.
 *
 *
 */

/*****************************************************************

 file: ece486_nco.h
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


#ifndef ECE486_NCO
#define ECE486_NCO

#define TWO32F 4294967296.0f // Floating point 2^32

typedef struct {
	unsigned int f0_step; // f0 step value for generating samples
	unsigned int theta_step; // Theta step value for generating samples
	float theta; // Track the actual floating point theta value of NCO
	float amp; // Amplitude of the output waveform
	float *lookup; // Pointer to lookup table
	int ctrlWord; // Track position in lookup table

} NCO_T;

NCO_T *init_nco(float f0, float theta);

void nco_get_samples(NCO_T *s, float *y, int n_samples);

void nco_set_frequency(NCO_T *s, float f_new);

void nco_set_phase(NCO_T *s, float theta);

void destroy_nco(NCO_T *s);


#endif
