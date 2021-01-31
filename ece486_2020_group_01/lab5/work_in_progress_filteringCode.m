 
% A filter is designed that allows 0.2 dB of ripple in the pass band of 
% and no more than 60 dB of rejection

%numbers
N = 15*1024;
%sampling 
Fs = 32e3;
% number of coefficients
M = 80; 

%bigger it is the smoother it is
beta = 8.3; 

%Frequency arrays (discrte tiem)
f = (0:N-1)/N;
%getting the corresponding neg values 
f(N/2+2:end) = f(N/2+2:end)-1;


%Setting up specs: 
figure (1); clf; %Frequency response
%First block spec
patch([0 0.2 0.2 0],[-100 -100 -80 -80], .9*[1 1 1]);
%Second block spec
patch([.4 0.5 0.5 .4],[-100 -100 -80 -80], .9*[1 1 1]);
%Pass band spec
patch([.25 .35 .35 .25],[-11 -1 1 -9],.9*[1 1 1]);
grid on;
hold on;

%designing the window 
%hd(f) 
%for passband; getting a value of one in the pass band
%Change to move edges
%ideal bandpass 
Hd = (abs(f)>0.235 & abs(f)<0.366).*(10.^( (10/.1)*(abs(f)-.35)/20));
%delay in the frequncy domain to make causal 
Hd = Hd.*exp(-j*2*pi*f*(M-1)/2);
plot(f,20*log10(abs(Hd)),'r');

%calculate the impluse response 
hd = ifft(Hd);
figure (2); clf; 
hold on;
n = 0:N-1;
stem(n,hd, '.')
max(imag(hd))
%to find the maximum 
%rounding imaginary componet. Ideally all
%real. If it does have an imag component 
%then hd is not symtric. 

%Making it into the FIR 
win = kaiser(M,beta)'; %beta sets how smooth 
%the window is 
%figure(3);
%plot(win,'.')
h = hd(1:M).*win;
plot(0:M-1,h,'r.')

%Checking
%calculate the acutal gain 
H = fft(h,N); 
figure(1);
plot(f(1:1:N/2),20*log10(abs(H(1:N/2))));

