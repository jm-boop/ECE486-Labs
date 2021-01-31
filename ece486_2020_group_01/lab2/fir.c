/*!
 * @file
 *
 * @brief ECE 486 Spring 2020 Lab 2
 *
 * @author ECE486 Lab Group 1

 */


#include "fir.h"
#include <stdio.h>
#include <stdlib.h>


FIR_T * init_fir(float *fir_coefs, int n_coef, int blocksize){
  // Allocate memory for an FIR_T sstruct
  FIR_T * s;

 s = (FIR_T*)malloc(sizeof(FIR_T));

   // Initialize variables
  s->fir_coefs = fir_coefs;
  s->blocksize = blocksize;
  s->n_coef = n_coef;
  s->buffer = (float*)malloc((n_coef)*sizeof(float));

  int i;
  for(i =0; i <n_coef;i++)
{
  s->buffer[i] = 0.0;
}
s->Index = 0;
return s;
}

void calc_fir(FIR_T*s,float *x, float *y){
  int k,n;
  for (n = 0; n < s->blocksize; n++){
      // copying xn to history buffer
    s->buffer[s->Index] = x[n];
    y[n] = 0.0;
  //suming everything
    for(k= 0; k< s->n_coef; k++){
        //sum everything in the convolution
      int place = s->Index-k;
y[n] += (s->fir_coefs[k]* s->buffer[place]);
}
s->Index++;
if(s->Index == (s->n_coef)){
  s->Index = 0;
}
}
}

  void destroy_fir(FIR_T*s){
    free(s->buffer);
    free(s);
s = NULL;
}
