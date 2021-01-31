% initialize sampling frequency (ksps)
Fs = 48e3;
f = linspace(0,0.5, 1e5);
z = exp(1i*2*pi.*f);

% initialize value and amount of impulse coefficients
coeff_val = .05;
coeff_num = 20;

% allocate space for impulse coefficients
h = zeros(1, coeff_num);

% assign impulse coefficients iteratively 
for n = 1:coeff_num
    h(n) = coeff_val;
end

% evaluate the filter gain
Hnum = polyval(h, z);
Hden = polyval(1, z);
H = Hnum./Hden;

% plot gain vs. frequency
plot(f .* 48e-1, db(H));
xlabel('freq (kHz)');
ylabel('gain (dB)');
ylim([-125 0]);