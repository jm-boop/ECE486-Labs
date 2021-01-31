#include <stdio.h>
#include <stdlib.h>
#include "running_meanJB.h" 

struct saved init_running_mean(int M, int blockSize)
{
	struct saved s;

	s.M = M;
	s.blockSize = blockSize;
	s.last = (float *)calloc(M - 1,sizeof(float));
	return s;
}

/*void fail(void) {
	printf("memory could not be allocated");
	exit(EXIT_FAILURE);
}*/

void calc_running_mean(float * x, struct saved s)
{
	int size = sizeof(blockSize);
	/* allocate space on heap to hold M floats,
	 * initialized to 0 
	 */
	float *y = (float *)calloc(size,sizeof(float));
	/* check if pointer is NULL */
	for (int i = 0; i <= size; i++){
		if (i < s.M){
			y[i] = (sumSlice(x,1,i) + sumSlice(s.last,i,sizeof(blockSize))/s.M);
			printf("%f ", y[i]);
		} else {
			y[i] = sumSlice(x,(i - s.M + 1),i) / s.M;
			printf("%f ", y[i]);
		}
	}
}

float sumSlice(float *a, int startIndex, int endIndex) 
{
	float sum = 0;

	for (int i = startIndex; i < endIndex; i++){
		sum = sum + a[i];
	}
	return sum;
}
	
