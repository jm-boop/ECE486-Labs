function [y,s] = calc_running_meanAN(x,s)
%%The function calc_running_mean calculates the running mean value of a
%vector. The function will calculate the mean of the vector 'x' and store
%the value in the vector y. The calulations are stored in the struture s.
%This incluses the number of samples, 'M', the last M-1 samples in the
%previous blocks, 'last' and the current index of the previous sample
%is stored in 'lastIndex', then the running sum of the M-1 samples is stored in 'previous'. 
%Example: NEED TO ADD AN EXAMPLE!

% Group one: John Bowen, Nate McNutt, Annalyse Nichols 
%ECE 486 lab one: running mean 
% put the sample vector to a colum vector (just in case)
x = x(:)

%index through the input array 
for n =1:size(x,1)
    %finding y at the value of n
    y(n,1) = (s.previous + x(n))/s.M;
    
    s.previous = s.previous - s.last(s.lastIndex);
    s.previous = s.previous + x(n);
    s.last(s.lastIndex) = x(n);
    s.lastIndex = s.lastIndex + 1; 
    
 % looping to the beginning    
    if (s.lastIndex == s.M)
        s.lastIndex = s.lastIndex - (s.M -1);
    end 
end 
end 