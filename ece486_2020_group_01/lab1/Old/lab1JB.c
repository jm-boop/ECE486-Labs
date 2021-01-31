#include <stdio.h>
#include <stdlib.h>
#include "running_meanJB.h"

int main(int argc, char * argv[]){
	struct saved test;
	test = init_running_mean(3,7);
	
	printf("%d, %d, %d\n", test.M, test.blockSize, test.last);
}
