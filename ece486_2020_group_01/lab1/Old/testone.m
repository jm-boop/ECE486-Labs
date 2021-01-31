
%% Test 1


% Set sample size
samples = 10;

% x(n) = vector of data to average
x = sin(1:1000)+1;

% Set blocksize
blocksize = 4;

% Initialize struct with given sample size and blocksize
s = init_running_mean(samples, blocksize);

% Calculate running mean
[y,s] = calc_running_mean(x,s)
[y,s] = calc_running_mean(x,s)
[y,s] = calc_running_mean(x,s)
[y,s] = calc_running_mean(x,s)

% The resulting value, stored in the output y, should be hovering around one.

