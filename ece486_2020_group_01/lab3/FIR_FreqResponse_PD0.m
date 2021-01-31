% John Bowen
% ECE 486
% Homework 3

%% 1.
f = linspace(-0.5,0.5, 1e5);
N=1001;
w=2.*pi.*[0:N-1]./N;

% set z to evaluate H(z) on unit circle
z = exp(1i*2*pi.*f);
z1 = exp(1i.*w);

% initialize zeros and poles with their imaginary and real parts
zeros = [0.99 .* exp(1i.* 2 .* pi .* 0.092 .* [-1,1]),...
        0.88 .* exp(1i.* 2 .* pi .* 0.35 .* [-1,1]),...
        0.85 .* exp(1i.* 2 .* pi .* 0.4 .* [-1,1]),...
        0.9 .* exp(1i.* 2 .* pi .* 0.05 .* [-1,1])];
     
poles = [0.899 .* exp(1i.*2.*pi.*0.1499 .* [-1,1]),...
         0.94 .* exp(1i.*2.*pi.*0.254 .* [-1,1]),...
         0.8 .* exp(1i.*2.*pi.*0.23 .* [-1,1]),...
         0.89 .* exp(1i.*2.*pi.*0.145 .* [-1,1])];

% getting the plot for pole-zero diagram
figure(1);
hold on
plot(z1)
plot(real(poles), imag(poles),'x')
plot(real(zeros), imag(zeros),'o')
title("Plot of pole-zero diagram");
xlabel("Re(z)");
ylabel("Im(z)");
axis square
axis([-1.5 1.5 -1.5 1.5])
hold off     
     
     

% return coefficients of polynomial whose roots are elements of argument
num = poly(zeros);
den = poly(poles);

% evaluate polynomials for z
numm = polyval(num, z);
denn = polyval(den, z);

% initialize and plot filter gain H(z)
figure(2);
patch([0, 0.1, 0.1, 0], [-60, -60, -40, -40], [1 0 1]);
alpha(0.3);
hold on;

H = numm./denn;
% Must have DC gain (where f = 0) of 1.0 (0 dB)
G = 1./max(abs(H));
H = G.*numm./denn;
plot(f, db(H));
xlim([0 .5]);
%ylim([-60 5]);
grid on;
xlabel("cycles/sample");
ylabel("filter gain (dB)");

[sos, g] = zp2sos(zeros, poles, G, 'up', 'inf');

a = sos(1, 1:3);
b = sos(1, (end - 1):end);

h = [b, a];

disp("float biquad_coefs[] = {")

for i = 1:length(h(:,1))-1
    fprintf("%g, %g, %g, %g, %g,\n", h(i,:));
    
end

fprintf("%g, %g, %g, %g, %g};\n", h(end,:));

% plot pole-zero diagram
% figure(2);
% n = 1024;
% w = (2.*pi.*[0:(n - 1)])./n;
% zw = exp(1i.*w);
% plot 
%% 3d)
% calc sequences and verify expressions give same results for 0<=n<=20

n = 0:20;
x1 = 2.*3.04.*(.8.^n).*cos(.723.*n - 1.05);
x2 = 3.*(.8.^n).*cos(.723.*n) + (.8.^n).*sin(.723.*n);
x3 = 5.67.*(.8.^(n + 1)).*sin(.723.*(n + 1)) + 1.89.*(.8.^n).*sin(.723.*n);