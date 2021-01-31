clear all; 
% Tests the function calc_running_mean.m

%%This test looks for when M is bigger than the sample size
x1 = [1 2 3 4 5 6 7 8 9 10];
s = init_running_meanAN(4,10)
[y,s] = calc_running_meanAN(x1,s)


