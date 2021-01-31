#include <stdio.h>
#include <stdlib.h>
#include "running_meanNM.h"

int main(int argc, char * argv[]){
	struct saved test;
	test = init_running_mean(7, 10);
/*	printf("%d, %d, %d\n", test.M, test.blockSize, test.last);
	//test = init_running_mean(6, 10);
	printf("%d, %d, %d\n", test.M, test.blockSize, test.last);
	for(int i = 0; i<test.M;i++){
		*test.last = (float)100.0;
		printf("Value @ %d: %f\n",test.last, (float)*test.last);
		test.last++;
	}

	float test2[9] = {1,2,3,4,5,6,7,8,9};

	//printf("%f\n", sumSlice(test2, 2, 5));
	float x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	float *out;
	out = calc_running_mean(x, test);
	for(int i = 0; i < test.M; i++){
		printf("%f, ", *(test.last+i));
	}
*/
	//Initialize an input number of samples
	float x[] = {1,2,3,4,4,4,4,4,4,4};
	
	//Initialize the output pointer
	float *y;

	//Save the struct saved returned from initialization function
	test = init_running_mean(3, 10);

	//Run 10 cycles of running mean calculations
	for(int i = 1; i<=10;i++){
		y = calc_running_mean(x, test);
	}

	printf("\n\n");
	
	//Verify that the output y actually is saved on the stack
	for(int j = 0; j<test.blockSize;j++){
		printf("%f, ", *(y+j));
	}
	
	printf("\n");

	//Call final function to free memory on the stack
	end_running_mean(y, test);

	printf("\n");
}
