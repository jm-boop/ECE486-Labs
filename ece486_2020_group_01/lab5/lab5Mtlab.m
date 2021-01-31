 
%%

%Lab Five Low pass filter to prevent alaising 
%when decimating by a factor of 10
%Group One: John Bowen, Jeremey Juybari, Nate McNutt, Annalyse Nichols
%Passband of 1000
%Stopband of 2199Hz 
% A filter is designed that allows 0.2 dB of ripple in the pass band of 
% and no more than 60 dB of rejection

%numbers
N = 16*1024;
%sampling 
Fs = 32e3;
% number of coefficients
%bigger it is the smoother it is
beta = 6; 
% Parks MacMillin equiripple
f = (0:N-1)/N;
M2 = 66;
F = [0 0.03125 0.06875 0.5] / 0.5;
A =[1 1 0 0];
d1 = 1 - 10^(-0.2/20);
d2 = 10^(-60/20);
W = [1/d1 1/d2];
hpm = firpm(M2-1, F, A, W);
Hpm = fft(hpm, N);
figure(1); clf;
%Second block spec patch for f = 0.125 to 0.5
patch([.06875 .5 .5 .06875],[-100 -100 -60 -60], .9*[1 1 1]);
%Pass band spec
patch([0.0 0.03125 0.03125 0.0],[-0.2 -0.2 0.2 0.2],.9*[1 1 1]);
grid on;
hold on;
plot(f, 20 * log10(abs(Hpm)), 'r');
xlim([0 0.5]);

for i = 1:length(hpm(:,1))-1
    fprintf("%g, %g, %g, %g, %g, \n", hpm(i,:));
    
end
fprintf("%g, %g, %g, %g, %g, \n", hpm(end,:));

