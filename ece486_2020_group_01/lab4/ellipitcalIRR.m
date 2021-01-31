%Ellipical IIR 
%for part 2 

%Sample Rate, Hz
Fs = 50e3;
%Pass Band, Hz 
%Divided by ten because thats what the lab made 
%it sound like we were suppose to do...
Fp = 11000;
%Stop band, Hz
%Divided by ten because thats what the lab made 
%it sound like we were suppose to do...
Fst= 12300;
%Passband ripple, dB
Ap = .5;
%Stop band attenuation. dB
As = 65;
match = 'both';
d = fdesign.lowpass(Fp,Fst,Ap,As,Fs)
Hd = design(d,'ellip','MatchExactly',match)
%Hd = design(d,'equiripple');
hfvt = fvtool(Hd);
%get function values
[b, a] = tf(Hd)
%calcualted gain 
gain = prod(Hd.ScaleValues)
%Create a function in the form of direct transfor 2
Hd = dfilt.df2t(b, a);
a = Hd.Denominator 
b = Hd.Numerator
z = roots(b)
p = roots(a)	

%Coeffincents for c-file
[sos, k] = zp2sos(z, p, gain, 'up');

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

% 
% N=1001;
% w=2*pi*[0:N-1]/N;
% z2 = exp(1i*w); 
% 
% iirpoles = p';
% iirzeros = z';
% 
% figure(2);
% hold on
% plot(z2)
% plot(real(iirpoles), imag(iirpoles),'x')
% plot(real(iirzeros), imag(iirzeros),'o')
% title("Plot of pole-zero diagram");
% xlabel("Re(z)");
% ylabel("Im(z)");
% axis square
% axis([-1.5 1.5 -1.5 1.5])
% hold off
% 
% % calculating the gain
% den = poly(iirpoles);
% num = poly(iirzeros);
% f = linspace(0,0.5,N);
% w = 2 * pi *f;
% z2 = exp(1i * w);
% figure(2);
% Hd = abs(polyval(num,z2) ./ polyval(den, z2));
% % scaling to get DC gain of 1
% Hz_Dc = (1/max(Hd)) .* Hd;
% 
% % num gives the coef and so does den
% % patch 
% %plot in dB
% Hdb = 20*log10(Hz_Dc);
% 
% 
% % This is to print the coefficients this came from Nates file
% % g is gain
% g = Hz_Dc;
% 
% % Create a and b coefficients using zp2sos
% 
% % NOTE the up and inf cause and error and we dont know why.
% [sos, k] = zp2sos(iirzeros, iirpoles, g, 'up');
% 
% %[sos, k] = zp2sos(iirzeros, iirpoles, g);
% 
% % Make the coefficients usable in c
% a = sos(:, end-1:end);
% b = sos(:, 1:3);
% 
% % Combine coefficient matrices
% h = [b, a];
% % disp(k)
% 
% disp("float biquad_coefs[] = {")
% 
% for i = 1:length(h(:,1))-1
%     fprintf("%g, %g, %g, %g, %g,\n", h(i,:));
%     
% end
% 
% fprintf("%g, %g, %g, %g, %g};\n", h(end,:));
