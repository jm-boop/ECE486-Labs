#ifndef RUNNING_MEANNM_H
#define RUNNING_MEANNM_H

struct saved{
	int M;
	int blockSize;
	float * last;
};

struct saved init_running_mean(int M, int blockSize);
float sumSlice(float *a, int startIndex, int endIndex);
float *calc_running_mean(float *x, struct saved s);
void end_running_mean(float *y, struct saved s);

#endif //RUNNING_MEANNM_H
