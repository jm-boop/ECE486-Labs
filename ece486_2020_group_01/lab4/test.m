
%Open file dialog to locate data file
%[file, path2File] = uigetfile('/Users/natemcnutt/Documents/ece486_2020_group_01/lab4/*.txt');

% Concatenate for complete file path to the specified file
%file2Plot = [path2File, file];
file2Plot = '/Users/natemcnutt/Documents/ece486_2020_group_01/lab4/nco.txt';

% Import the data
output = importdata(file2Plot);

% Generate x values for length generated
x = 0:511;

% Create plot
hold on;
plot(x, output)