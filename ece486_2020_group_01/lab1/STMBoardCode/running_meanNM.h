#ifndef RUNNING_MEANNM_H
#define RUNNING_MEANNM_H

struct saved{
	int M;
	int blockSize;
	float * last;
};

struct saved init_running_mean(int M, int blockSize);
float sumSlice(float *a, int startIndex, int endIndex);
void calc_running_mean(float *y, float *x, struct saved s);
void end_running_mean(struct saved s);

#endif //RUNNING_MEANNM_H
