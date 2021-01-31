#include "running_meanAN.h"

for (i=0;i<s->blockSize;i++0)

// storage

y[i] = (s->previous + x[i])/ s->M;

//Update history

s->previous -= (s->last[s->lastIndex])*(s->last[s->lastIndex]);
s->previous += x[i];
s->last[s->lastIndex] = x[i];
s-> lastIndex += 1;

//Check index is at the end
if(s->lastIndex == (s->M-1)){
s->LastIndex -= (s->M-1);

}
