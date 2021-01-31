/*!
*@filte
*
*@brief Inplents our calc_fir() function and arm_fir functioon for the
*STM board
*
*@author ECE486 Group one
*@author John Bowen, Jeremy Juybari, Nate McNutt, Annalyse Nichols
*
*This file takes a wavegen input and does an FIR filter on the singal,
*outputting the filtered signal. Two FIR filters are implemented. Our
*fir fucntion developed in Task two and the FIR filter developed by Arm
*
*/
//Include files
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include "ece486_fir.h"
#include "arm_math.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//button press detection
extern FlagStatus KeyPressed;

int main(void)
{
//setting up variables
float *h,*pState, *x, *y,*y1;
char lcd_str[8];
//Setting up structs
FIR_T *s = (FIR_T *)malloc(sizeof(FIR_T *));
arm_fir_instance_f32 *s1 = (arm_fir_instance_f32 *)malloc(sizeof(arm_fir_instance_f32 *));

//initalize the board
initialize_ece486(FS_48K, MONO_IN, STEREO_OUT, HSE_EXTERNAL_8MHz);

//Number of coefficients
uint32_t number =20;
//Number of samples
uint32_t blocksize = getblocksize();
//Setting up mememory
h = (float *)malloc(number*sizeof(float));
x = (float *)malloc(blocksize*sizeof(float));
y = (float *)malloc(blocksize*sizeof(float));
y1 = (float *)malloc(blocksize*sizeof(float));
//Comment out pstate line to get our code to run;
//make sure not commeneted to run their code
pState = (float *)malloc(blocksize-1+number*sizeof(float));

//creating a whole bunch of impulses
for(int i = 0; i<number;i++){
h[i] = 1.0/20.0;
}
//status check just in case
if(x==NULL || y==NULL||y1==NULL||h==NULL){
  flagerror(MEMORY_ALLOCATION_ERROR);
  while(1);
}
//initialize
s = init_fir(h,number,blocksize);
arm_fir_init_f32(s1,number,h,pState,blocksize);

while(1){
  //get input
  getblock(x);

  DIGITAL_IO_SET();
//do stuff with the data
  arm_fir_f32(s1,x,y1,blocksize);

  DIGITAL_IO_RESET();

  DIGITAL_IO_SET();
//do stuff with the data
calc_fir(s,x,y);

DIGITAL_IO_RESET();
//output data
putblockstereo(y,y1);
}
}
