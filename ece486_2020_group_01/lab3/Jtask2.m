%% Problem 1
% I got most of this code from the lab 3 briefing with Hummels.
N=1001;
w=2*pi*[0:N-1]/N;
z = exp(1i*w);

% what poles do we want the first number is the magnitude
% Number of poles = number of zeros
% 0.30 is the frequency and [-1,1] is for the conjugate pair
iirpoles = [
        0.899 * exp(1i*2*pi*0.1499 * [-1,1]),...
        0.94 * exp(1i*2*pi*0.254 * [-1,1]),...
        0.8 * exp(1i*2*pi*0.23 * [-1,1]),...
        0.89 * exp(1i*2*pi*0.145 * [-1,1])
        
];

% same structure as the poles but with zeros
iirzeros = [
        0.99 * exp(1i* 2 * pi * 0.092 * [-1,1]),...
        0.88 * exp(1i* 2 * pi * 0.35 * [-1,1]),...
        0.85 * exp(1i* 2 * pi * 0.4 * [-1,1]),...
        0.9 * exp(1i* 2 * pi * 0.05 * [-1,1])
        
];

iirpoles = iirpoles';
iirzeros = iirzeros';
% getting the plot for pole-zero diagram
figure(1);
hold on
plot(z)
plot(real(iirpoles), imag(iirpoles),'x')
plot(real(iirzeros), imag(iirzeros),'o')
title("Plot of pole-zero diagram");
xlabel("Re(z)");
ylabel("Im(z)");
axis square
axis([-1.5 1.5 -1.5 1.5])
hold off

% calculating the gain
den = poly(iirpoles);
num = poly(iirzeros);
f = linspace(0,0.5,N);
w = 2 * pi *f;
z = exp(1i * w);
figure(2);
Hz = abs(polyval(num,z) ./ polyval(den, z));
% scaling to get DC gain of 1
Hz_Dc = (1/max(Hz)) .* Hz;

% num gives the coef and so does den
% patch 
%plot in dB
Hdb = 20*log10(Hz_Dc);
figure(2);
hold on;
patch([0 .1 .1 0], [-60, -60, -40, -40], 0.8*[1, 1, 1])
patch([.35 .5 .5 .35], [-60, -60, -50, -50], 0.8*[1, 1, 1])
patch([.15 .2 .25 .25 .2 .15], [-1, -6, -1, 1, -4, 1], 0.8*[1,1,1])
plot(f,Hdb);
title("Plot of Gain for Problem 1");
xlabel("frequency, f");
ylabel("Gain, dB");
hold off;


% This is to print the coefficients this came from Nates file
% g is gain
g = Hz_Dc;

% Create a and b coefficients using zp2sos

% NOTE the up and inf cause and error and we dont know why.
%[sos, k] = zp2sos(iirzeros, iirpoles, g, 'up', 'inf');

[sos, k] = zp2sos(iirzeros, iirpoles, g);

% Make the coefficients usable in c
a = sos(:, end-1:end);
b = sos(:, 1:3);

% Combine coefficient matrices
h = [b, a];
% disp(k)

disp("float biquad_coefs[] = {")

for i = 1:length(h(:,1))-1
    fprintf("%g, %g, %g, %g, %g,\n", h(i,:));
    
end

fprintf("%g, %g, %g, %g, %g};\n", h(end,:));