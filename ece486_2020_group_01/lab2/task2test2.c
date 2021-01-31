/*!
*@file
*@brief ECE 486 IRR filter test two.
*
*@author ECE486 Lab Group 1
*@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
*
*@date Febuary 2020
*
*This file tests the BIQUAD subroutines
*/

#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
    // Set up biquad filter for testing
    int sections = 3;
    float g = 1;
    //float *biquad_coefs = (float *)calloc((sections*5), sizeof(float));
    float biquad_coefs[] = {1, 1.618, 1, -1.5371, 0.9025,1,-0.618,1,0.0,-0.81, 1, -0.789, 1.67, -1.34, 0.67};
    int blockSize = 20;

    float x[20] = {3, 1.5, -1.8,0.2,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float y[20];
    // Initialize biquad struct
    BIQUAD_T *s = init_biquad(sections, g, biquad_coefs, blockSize);

    // Test calc biquad function
    calc_biquad(s, x, y);
    for (int i = 0; i < s->blocksize; i++) {
        printf("%f\n", y[i]);
    }

    // Clear memory
    destroy_biquad(s);


    return 0;
}
