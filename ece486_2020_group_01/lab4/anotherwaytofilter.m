[b,a] = ellip(5,.5,65,[.194 .246]);
freqz(b,a)
Hd = dfilt.df2t(b, a)
a = Hd.Denominator 
b = Hd.Numerator
z = roots(b)
p = roots(a)		
%gain = prod(Hd.ScaleValues)
iirpoles = p';
iirzeros = z';
% fvt = fvtool(sos,d,'Fs',50000);
% legend(fvt,'ellip','designfilt')

% calculating the gain
den = poly(iirpoles);
num = poly(iirzeros);
f = linspace(0,0.5,N);
w = 2 * pi *f;
z2 = exp(1i * w);
figure(2);
Hd = abs(polyval(num,z2) ./ polyval(den, z2));
% scaling to get DC gain of 1
Hz_Dc = (1/max(Hd)) .* Hd;

% num gives the coef and so does den
% patch 
%plot in dB
Hdb = 20*log10(Hz_Dc);


% This is to print the coefficients this came from Nates file
% g is gain
g = Hz_Dc;

% Create a and b coefficients using zp2sos

% NOTE the up and inf cause and error and we dont know why.
[sos, k] = zp2sos(iirzeros, iirpoles, g, 'up');

%[sos, k] = zp2sos(iirzeros, iirpoles, g);

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

% %%
% [A,B,C,D] = ellip(3,.5,65,[.12 .15]);
% d = designfilt('bandpassiir','FilterOrder',20, ...
%     'PassbandFrequency1',970,'PassbandFrequency2',1230, ...
%     'PassbandRipple',.5, ...
%     'StopbandAttenuation1',65,'StopbandAttenuation2',65, ...
%     'SampleRate',50000)
% sos = zp2sos(C,D)
% fvt = fvtool(sos,d,'Fs',50000);
% legend(fvt,'ellip','designfilt')
