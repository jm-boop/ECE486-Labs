%%Test script for calc_running_mean
%The script calls the function calc_running_mean with arguments of various dimensions and size 
%to ensure that the script works as expected under all conditions

%Authors: John Bowen, Nate McNutt, Annalyse Nichols
%ECE486 Group 1
%Lab One
clear all; 

%% test one M<blocksize
%y should be: 

%  Columns 1 through 4
% 
%     0.3333    1.0000    2.0000    3.0000
% 
%   Columns 5 through 8
% 
%     4.0000    5.0000    6.0000    7.0000
% 
%   Columns 9 through 10
% 
%     8.0000    9.00000
% s = 
%   struct with fields:
% 
%             M: 3
%     blocksize: 10
%          last: [9 10]

x1 = [1 2 3 4 5 6 7 8 9 10];
s = init_running_mean(3,10);
%should output y = [1/3 1 2 3 4 5 6 7 8 9]
[y,s] = calc_running_mean(x1,s)

%% test two
%y should be: 
 
%   Columns 1 through 6
% 
%     6.6667    4.3333    2.0000    3.0000    4.0000    5.0000
% 
%   Columns 7 through 10
% 
%     6.0000    7.0000    8.0000    9.0000
% s = 
%   struct with fields:
% 
%             M: 3
%     blocksize: 10
%          last: [9 10]

x2 = [1 2 3 4 5 6 7 8 9 10];
%passing same input second time should output 
%y = [20/3 13/3 2 3 4 5 6 7 8 9]
[y,s] = calc_running_mean(x2,s)

%% test three M = blocksize 
% y  should be: 
% 
%     0.3333    3.3333 
% s = 
% struct with fields:
% 
%             M: 3
%     blocksize: 3
%          last: [0 0]
x1 = [1 2 3];
x2 = [4 5 6];
x3 = [7 8 9];
s = init_running_mean(3,3)
[y1,s] = calc_running_mean(x1,s);
[y2,s] = calc_running_mean(x2,s);
[y3,s] = calc_running_mean(x3,s);

y = [y1:y2:y3]
