#include <stdio.h>
#include <stdlib.h>
#include "running_meanNM.h"

struct saved init_running_mean(int M, int blockSize){
	struct saved s;
	s.M = M;
	s.blockSize = blockSize;
	s.last = (float *)calloc(M, sizeof(float));
	//Initialize the values on the stack to zero
	return s;

}


float sumSlice(float *a, int startIndex, int endIndex){
	float sum = 0;

	for(int i = startIndex; i < endIndex; i++)
	{
		sum = sum + a[i];
		//printf("%f, ", *(result+i));
	}

	return sum;

}


float *calc_running_mean(float *x, struct saved s){
	
	//Initialize y array
	float *y;

	//Allocate memory on the stack to return a pointer to output y
	y = (float *)calloc(s.blockSize, sizeof(float));

	//For debugging purposes print out s.last
	for(int j = 0; j < s.M; j++){
		printf("%f, ", *(s.last+j));
	}
	printf("\n");
	
	//Average over all n values in x
	for(int n = 0; n < s.blockSize;n++){
		
		if(n < s.M){
			//Overlap the averaging into the s.last block
			y[n] = (sumSlice(s.last, n+1, s.M)+sumSlice(x, 0, n+1))/s.M;	
			//printf("%f, ", y[n]);
		}else{
			//Just do the normal averaging over s.M samples
			y[n] = sumSlice(x, n-(s.M-1), n+1)/s.M;
		}
		
		//Print out the value of output to verify
		printf("%f, ", y[n]);
	
	}

	//Save the last M elements to s struct
	for(int i = 0; i < s.M;i++){
		s.last[i] = x[s.blockSize-s.M+i];
		//printf("%f, ", s.last[i]);
	}

	printf("\n\n");
	
	//Return the pointer to the allocated output location on the stack
	return y;
	
}


void end_running_mean(float *y, struct saved s){
	//Free memory allocated
	free(y);
	free(s.last);
	printf("Memory has been freed.");
}
