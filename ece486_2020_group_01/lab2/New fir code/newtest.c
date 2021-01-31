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
f2 = init_fir(h2,2,4);
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
