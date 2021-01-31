%% script for designing biquad filter coefficients
figure(1);
% Poles
pls = [0.8*exp(1i*2*pi*[-.125,0.125]),...
    0.8*exp(1i*2*pi*[-.125,0.125]),...
    0.8*exp(1i*2*pi*[-.25,0.25]),...
    0.8*exp(1i*2*pi*[-.25,0.25])];

% Zeros
zs =[0.99*exp(1i*2*pi*[0.1, -0.1]), ...
    0.8*exp(1i*2*pi*[0.2, -0.2]), ...
    0.99*exp(1i*2*pi*[0.35, -0.35])...
    ];

w = linspace(0, 2*pi, 1001);
unit = 1*exp(1i*2*pi*w);
plot(real(unit), imag(unit), real(pls), imag(pls), 'X', real(zs), imag(zs), 'O')
axis square
xlabel('Real, z')
ylabel('Imaginary, z')
grid on
%% Create the filter gain
figure(2);



% Create polynomials, a and b found from the coefficients
b = poly(zs); %b
a = poly(pls); %a

% Change z->f
f = linspace(0, 0.5, 1001);
z = exp(1i*2*pi*f);

% Evaluate polynomials for frequency
zpv = polyval(b, z);
plv = polyval(a, z);

% Create gain function
H = zpv./plv;

% Calculate g such that max gain is 1 or 0dB
g = 1/max(abs(H))

% Plot the answer, use patch to mark out pass and reject bands
hold on;
patch([0, .1, .1, 0], [-60, -60, -40, -40], 0.9*[1,1,1]);
patch([.35, .5, .5, .35], [-60, -60, -50, -50], 0.9*[1,1,1]);
patch([.15, .2, .25, .25, .2, .15], [-1, -6, -1, 1, -4, 1], 0.9*[1,1,1])
plot(f, 20*log10(abs(H)*g))
hold off;

%Label the plot
title("Q1 Filter Gain Characteristic")
xlabel("Normalized Frequency, f")
ylabel("Gain, dB")


%% Convert poles and zeroes into biquad coefficients

% Poles
pls = [0.8*exp(1i*2*pi*[-.125,0.125]),...
    0.8*exp(1i*2*pi*[-.125,0.125]),...
    0.8*exp(1i*2*pi*[-.25,0.25]),...
    0.8*exp(1i*2*pi*[-.25,0.25])];

% Zeros
zs =[0.99*exp(1i*2*pi*[0.1, -0.1]), ...
    0.8*exp(1i*2*pi*[0.2, -0.2]), ...
    0.99*exp(1i*2*pi*[0.35, -0.35])...
    ];

% Create a and b coefficients using zp2sos
[sos, k] = zp2sos(zs, pls, g, 'up', 'inf');

% Make the coefficients usable in c
a = sos(:, end-1:end);
b = sos(:, 1:3);

% Combine coefficient matrices
h = [b, a];
disp(k)

disp("float biquad_coefs[] = {")

for i = 1:length(h(:,1))-1
    fprintf("%g, %g, %g, %g, %g,\n", h(i,:));
    
end

fprintf("%g, %g, %g, %g, %g};\n", h(end,:));
