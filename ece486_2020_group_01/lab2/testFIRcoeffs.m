%%
x = [ 1 2]
h = [ 1 2 3 4]
y = conv(x,h)
stem(y)
xlim([-.5 2.5])
ylim( [ 0 5])
xlabel('Input values')
ylabel('Impulse')
%%
x = [20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1]
h = [.05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05 .05];
y = conv(x,h)
b = fir1(20,0.05);
freqz(b,1)
%freqz(y)
%stem(y)
%xlim( [0 20])
% xlabel('Input values')
% ylabel('Impulse')
%%
x = [.10 .9 .8 .7]
h = [.1 .1];
y = conv(x,h)

stem(y)
xlim( [0 4.5])
xlabel('Input values')
ylabel('Impulse')