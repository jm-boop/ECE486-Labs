#include "running_meanAN_test.h"
#include <stdio.h>
#include <stdlib.h>

M_struct * init_runningMean(uint32_t M, uint32_t blocksize){
  M_struct *s;

  s = (M_struct *) malloc(sizeof(M_struct));

  s.M = M;
  s.blocksize = blocksize;
  s.previous = 0;
  s.lastIndex =0;
  s.last = calloc(M-1,sizeof(float));

  return s;

}

float * calc_runningMean(float* x, M_struct *s){
	//allocating
	float * y;
	y= (float *) calloc(s->blocksize,sizeof(float));

	uint32_t i;

	for(i=0; i<s.blocksize;i++){
		//storage
		y[i] = (s.previous + x[i])/s.M;
		//Update
		s.previous-= (s.last[s.lastIndex])*(s.last[s.lastIndex]);
		s.previous += x[i];
		s.last[s.lastIndex] = x[i];
		s.lastIndex +=1;
		//Check
		if(s.lastIndex == (s.M-1)){
			s.lastIndex -= (s.M-1);

		}	
	}
	return y;
}

void end_meanValue(M_struct*s, float * y){
	//free memory
	free(y);
	free(s.last);
	free(s);
}
