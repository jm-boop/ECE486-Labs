
%%
%calc_running_mean.m
function[y,s] = calc_running_mean(x,s)

%filling each range
for b = 1:s.blocksize
   s.block(b) = x(s.j+b)
end
for b = 1:s.blocksize
    last= s.lastM(mod(b+s.j-1,s.M)+1) 
    last = s.block(b);
    if b ==1
        s.y(b) = s.y(s.blocksize)+s.block(b)/s.M-last/s.M;
    end
end
s.j = s.j+s.blocksize;
y = s.y;
end 
%test.m

%%%%%%%
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

