/*
 **@file
 *
 *@brief ECE 486 IRR filter code
 *
 *@author ECE486 Lab Group 1
 *@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
 *
 *@date Febuary 2020.
 *
 * Program to flash the board.
 */

#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"

#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ece486_nco.h"
#include <stdint.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include <stdint.h>

extern FlagStatus KeyPressed;   // Use to detect button presses

int main(void)
{
	int nsamp;
	float *input, *output1, *output2;
	int counter=0;
	int i = 0;
  /*
   * Set up ADCs, DACs, GPIO, Clocks, DMAs, and Timer
   *
   * If your discovery board has been modified to support the external
   * HSE reference, use the (better quality crystal controlled) HSE
   * configuration.  If not, you can use the MSI reference, and things
   * should still work -- but your clocks will drift over time.
   *
   * The MSI (Medium speed internal) reference is the on-chip RC oscillator
   *
   * The HSE (High speed external) reference is an external clock, either
   * provided through the external connector, or (if your board is modified)
   * from the crystal reference source generated by the other processor
   * on the development board.
   */
  //initialize_ece486(FS_50K, MONO_IN, STEREO_OUT, MSI_INTERNAL_RC);
  initialize_ece486(FS_32K, MONO_IN, STEREO_OUT, HSE_EXTERNAL_8MHz);

  /*
   * Allocate Required Memory
   */
  //setblocksize(1024); // Size of the fft
  nsamp = getblocksize();
  int button_count =0;
  int f0 = 0;
  input = (float *)malloc(sizeof(float)*nsamp);
  output1 = (float *)malloc(sizeof(float)*nsamp);
  output2 = (float *)malloc(sizeof(float)*nsamp);
  float32_t *fft_output = (float32_t *)calloc(nsamp, sizeof(float32_t));
  float32_t *decBlock1 = (float32_t *)calloc(nsamp/10, sizeof(float32_t));
  float32_t *decBlock2 = (float32_t *)calloc(nsamp/10, sizeof(float32_t));

  // Set up NCO waveforms
  NCO_T *s1 = init_nco(11000/32000, 0.0);
  NCO_T *s2 = init_nco(11000/32000, 3*M_PI/2);
  s1->amp = 0.99;
  s2->amp = 0.99;

  // Set up FIR decimation filter
  uint16_t numTaps = 66;
  uint8_t M = 10;
  float32_t pCoeffs[] = {0.000894733, 0.0010742, 0.00159809, 0.0021691, 0.00272184,
0.0031718, 0.00342232, 0.00336847, 0.00291526, 0.00198869,
0.000552561, -0.00137972, -0.00373259, -0.00636332, -0.00906144,
-0.0115575, -0.0135376, -0.0146663, -0.0146133, -0.0130853,
-0.00985679, -0.00480003, 0.00209268, 0.0106942, 0.0207392,
0.0318318, 0.0434665, 0.0550586, 0.0659845, 0.0756268,
0.0834209, 0.0888985, 0.0917246, 0.0917246, 0.0888985,
0.0834209, 0.0756268, 0.0659845, 0.0550586, 0.0434665,
0.0318318, 0.0207392, 0.0106942, 0.00209268, -0.00480003,
-0.00985679, -0.0130853, -0.0146133, -0.0146663, -0.0135376,
-0.0115575, -0.00906144, -0.00636332, -0.00373259, -0.00137972,
0.000552561, 0.00198869, 0.00291526, 0.00336847, 0.00342232,
0.0031718, 0.00272184, 0.0021691, 0.00159809, 0.0010742,
0.000894733};

  uint32_t blockSize = nsamp;

  float32_t *pState = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));
  float32_t *pState2 = (float32_t *)calloc(numTaps+blockSize-1,sizeof(float32_t));

  arm_fir_decimate_instance_f32 s;
  arm_fir_decimate_instance_f32 s3;

  arm_fir_decimate_init_f32(&s, numTaps, M, pCoeffs, pState, blockSize);
  arm_fir_decimate_init_f32(&s3, numTaps, M, pCoeffs, pState2, blockSize);

  // Check for memory allocation errors
  if (input==NULL||output1==NULL||output2==NULL) {
    flagerror(MEMORY_ALLOCATION_ERROR);
    while(1);
  }

  /*
   * Normally we avoid printf()... especially once we start actually
   * processing streaming samples.  This is here to illustrate the
   * use of printf for debugging programs.
   *
   * To see the printf output, connect to the ST-Link serial port.
   * Use: 115200 8N1
   */
  printf("Starting execution using %d samples per input block.\n",nsamp);
  /*
   * Infinite Loop to process the data stream, "nsamp" samples at a time
   */
  while(1){
    /*
     * Ask for a block of ADC samples to be put into the working buffer
     *   getblock() will wait here until the input buffer is filled...  On return
     *   we work on the new data buffer... then return here to wait for
     *   the next block
     */
    getblock(input);
    counter++;
    /*
     * signal processing code to calculate the required output buffers
     */

    DIGITAL_IO_SET(); 	// Use a scope on PD0 to measure execution time

    	// Generate NCO with proper center frequency from button
	nco_set_frequency(s1, f0/32000.0);
	nco_set_frequency(s2, f0/32000.0);

	// Code to calculate NCO waveform samples (+-exp(2*pi*f0))
	nco_get_samples(s1, output1, nsamp);
	nco_get_samples(s2, output2, nsamp);

	// Multiply input by 2 NCO waveforms (MIXER)
	for (i = 0; i < nsamp; i++) {
		output1[i] = input[i] * output1[i];
		output2[i] = input[i] * output2[i];
	}

	// Filter
	arm_fir_decimate_f32(&s, output1, decBlock1, blockSize);
    	arm_fir_decimate_f32(&s3, output2, decBlock2, blockSize);

	if (counter > 9) {
		// CALCULATE FFT IN HERE
		counter = 0;
	}

	for (i = 0; i < (nsamp/M); i++) {
		output1[counter*nsamp/M+i] = decBlock1[i];
		output2[counter*nsamp/M+i] = decBlock2[i];
	}



    DIGITAL_IO_RESET();	// (falling edge....  done processing data )

    /*
     * pass the processed working buffer back for DAC output
     */
    putblockstereo(output1, output2);
    // Cycle through modulation types by pushing the button
    if (KeyPressed) {
	KeyPressed = RESET;
	button_count++;

	// If no more encoding types then restart
	if (button_count > 15) {
		button_count = 0;
	}
f0=button_count*1000;
	// Clear the LCD display
  char dispString[8];
  sprintf(dispString,"%d",f0);
  BSP_LCD_GLASS_DisplayString((uint8_t *)"        ");
	BSP_LCD_GLASS_DisplayString((uint8_t *)dispString);

	// Draw to the display the correct method of keying


}
}

//  destroy_biquad(s);
}
