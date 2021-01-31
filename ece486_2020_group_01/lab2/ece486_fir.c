/*!
 * @file
 *
 * @brief ECE 486 Spring 2020 Lab 2
 *
 * @author ECE486 Lab Group 1

 */

#include "ece486_fir.h"
#include <stdio.h>
#include <stdlib.h>

FIR_T * init_fir(float *fir_coefs, int n_coefs, int blocksize){
  // Allocate memory for an FIR_T sstruct
  FIR_T * s;
  s = (FIR_T *) malloc(sizeof(FIR_T));

  // Initialize variables
  s->fir_coefs = fir_coefs;
  s->n_coefs = n_coefs;
  s->blocksize = blocksize;
  s->history = (float *) malloc((n_coefs)*sizeof(float));

  int i;
  for(i =0; i < n_coefs;i++)
  {
    s->history[i] = 0.0;
  }
  s->Index = 0;
  return s;
}

void calc_fir(FIR_T*s,float *x, float *y){
  int k,n;
  for (n = 0; n < s->blocksize; n++){
    // copying xn to history buffer
    s->history[s->Index] = x[n];
    y[n] = 0.0;
    //sum everything in the convolution
    for ( k =0; k < s->n_coefs; k++){
      int index = s->Index -k;
      //looping
      index = index>=0 ? index : index+s->n_coefs;
      y[n] += (s->fir_coefs[k] * s->history[index]);

    }
    s->Index++;
    if(s->Index == (s->n_coefs)){
      s->Index = 0;

    }
  }
}
void destroy_fir(FIR_T*s){
  free(s->history);
  free(s);
  s = NULL;
}
