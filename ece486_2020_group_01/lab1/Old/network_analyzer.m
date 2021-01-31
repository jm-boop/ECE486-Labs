%%
figure(1)
data = readmatrix('networkanalyzer.csv');

gain = data(:,2);

frequency = data(:,1);
x = [ 1240 2436 ]
y = [ -43.91 -31.94 ] 
plot(frequency, gain, x, y, 'o')
text1 = '1.2kHz'
text2 = '2.4kHz'
text(1245, -45,text1)
text(2400,-33,text2)


xlabel ('Frequency, Hz') 
ylabel ('Gain, dB')
%%
figure(2) 
data = readmatrix('scope_trace.csv');

time = data(:,1);
chone = data(:,2);
chtwo = data(:,3);



plot(time, chone, time, chtwo)
xlabel ('Time, Sec') 
ylabel ('Voltage, V')
xlim ([0 5e-3])