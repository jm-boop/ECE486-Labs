#ifndef RUNNING_MEAN_H
#define RUNNING_MEAN_H

struct saved{
	int M;
	int blockSize;
	int * last;
};

struct saved init_running_meanJB(int M, int blockSize);
void calc_running_meanJB(float * x, struct saved s);

#endif //RUNNING_MEAN_H
