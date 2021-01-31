/*!
*@file
*
*@brief ECE 486 Spring 2020 FIR TESTING
*
*@author ECE486 Group 1
*@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
*
*@date Febuary 2020
*
*This file tests the FIR filter
*
*/

#include "ece486_fir.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
//Testing when n_coefs is bigger than the blocksize :)
//MATLAB says we should get:
// y(0) = 1.000
//y(1) = 4.000

	FIR_T *f;

//impuse response
	float h0[4] = {1,2,3,4};
//input
	float x1[2] = {1,2};
//output
	float y1[2] = {0,0};
//initialize filter
	f = init_fir(h0,4,2);
//Calculation time
	calc_fir(f,x1,y1);
//Display values so we can see them

	int i;
	for (i=0; i<2; i++){
		printf("y(%d) = %f\n",i,y1[i] );
	}
  printf("\n");

  destroy_fir(f);

//Test with the 20 flipping coefficients, when n_coefs = blocksize
//Declare filter
//MATLAB says we should get:
//y(0) = 10
//y(1) = 19.5
//y(2) = 28.5
//y(3) = 37
//Y(4) = 45
//y(5) = 52.5
//y(6) = 59.5
//y(7) = 66
//y(8) = 72
//y(9) = 77.5
//y(10) = 82.5
//y(11) = 87
//y(12) = 91
//y(13) = 94.5
//y(14) = 97.5
//y(15) = 100
//y(16) = 102
//y(17) = 103.5
//y(18) = 104.5
//y(19) =105
FIR_T *f1;
//impulse
float h1[20] = {.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5,.5};
//input
float x2[20] = {.20,.19,.18,.17,.16,.15,.14,.13,.12,.11,.10,.9,.8,.7,.6,.5,.4,.3,.2,.1};
//output
float *y2 = x2;

//initialize the filter
f1 = init_fir(h1,20,20);
//calculation time
calc_fir(f1,x2,y2);

//Display values so we can see them
for(i= 0; i<20; i++){
printf("y(%d) = %f\n",i,y2[i]);
}
printf("\n");
destroy_fir(f1);

//Test when n_coefs is less than blocksize
//Declare fitler
FIR_T *f2;
//impuse response
float h2[2] = {.1,.1};
//input
float x3[4] = {.10,.9,.8,.7};
//output
float *y3 = x3;

//initial filter
f2 = init_fir(2,2,4);
//calc time
calc_fir(f2,x3,y3);

//Display values so we can see them
for (i=0; i<4; i++){
  printf("y(%d) = %f\n", i,y3[i]);
}
printf("\n");
destroy_fir(f2);

return 0;
}
