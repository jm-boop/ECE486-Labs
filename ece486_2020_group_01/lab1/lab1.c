#include <stdio.h>
#include <stdlib.h>
#include "running_mean.h"

int main(int argc, char * argv[]){
	struct saved s;
	s = init_running_mean(7, 10);
	
	//Initialize an input number of samples
	float x[] = {1,2,3,4,4,4,4,4,4,4};
	
	//Initialize the output pointer
	float *y=(float *)calloc(10, sizeof(float));

	//Save the struct saved returned from initialization function
	s = init_running_mean(3, 10);

	//Run 10 cycles of running mean calculations
	for(int i = 1; i<=10;i++){
		calc_running_mean(y, x, s);
	}

	printf("\n\n");
	
	//Verify that the output y actually is saved on the stack
	for(int j = 0; j<s.blockSize;j++){
		printf("%f, ", *(y+j));
	}
	
	printf("\n");

	//Call final function to free memory on the stack
	end_running_mean(y, s);

	printf("\n");
}
