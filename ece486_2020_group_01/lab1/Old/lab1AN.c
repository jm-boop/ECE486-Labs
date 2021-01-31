#include "running_meanAN.c"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argument, char *average[]){
float x1[7] = {1,2,3,4,5,6,7};
M_struct *s1 = init_runningMean(3,7);
float *y1 = calc_runningMean(x1,s1);

//display
int i;
for (i = 0, i<7; i++){
print("%f", y1[i]);
}
printf("\n\n");
}
