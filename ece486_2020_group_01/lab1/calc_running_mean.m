%%The function calc_running_mean calculates the running mean value of a
%vector. The function will calculate the mean of the vector 'x' and store
%the value in the vector y. The calulations are stored in the struture s.
%This incluses the number of samples, 'M', the last M-1 samples in the
%previous blocks, 'last' and the current index of the previous sample

%Example: 
% If s = 
%      
%             M: 3
%     blocksize: 10
%          last: [9 10]
% 
% and x equals: 
% x1 = [1 2 3 4 5 6 7 8 9 10]
% then [y,s] = calc_running_mean(x,s)
% returns y = [ 0.3333 1.0000 2.0000 4.0000 5.0000 6.0000 8.0000 9.0000

% Group one: John Bowen, Nate McNutt, Annalyse Nichols 
%ECE 486 lab one: running mean 
function [y,s] = calc_running_mean(x,s)

%x = x(:);

%allocate space for output array
y = zeros(1,size(x,2));

%index through the input array 
for n = 1:size(x,2)
    %handleing loop: goes around to the begining of the index buffer.  
       if(n < s.M)
           %calculate mean for indices where s.M reaches back into last
           %input array
           %calculates y(n)
           y(n) = sum([x(1:n) s.last(n:end)])./s.M;
       else
           %calculate mean for indices where s.M is within current input
           %array
           %calcualtes y(n)
           y(n) = sum(x((n - s.M + 1):n))./s.M;
       end
end
%update s.last array for next input array
s.last = x((end - (s.M - 2)):end);
end

% Another way; a little less efficient 
% function [y,s] = calc_running_meanAN(x,s)
% 
% % put the sample vector to a colum vector (just in case)
% x = x(:)
% 
% %index through the input array 
% for n =1:size(x,1)
%     %finding y at the value of n
%     y(n,1) = (s.previous + x(n))/s.M;
%     
%     s.previous = s.previous - s.last(s.lastIndex);
%     s.previous = s.previous + x(n);
%     s.last(s.lastIndex) = x(n);
%     s.lastIndex = s.lastIndex + 1; 
%     
%  % looping to the beginning    
%     if (s.lastIndex == s.M)
%         s.lastIndex = s.lastIndex - (s.M -1);
%     end 
% end 
% end 

