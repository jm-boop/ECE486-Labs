M=1280; %size of fft array
beta = 2;
win = kaiser(M,beta)';
%print out coefs
for i = 1:length(M(:,1))-1
    fprintf("%g, %g, %g, %g, %g, \n", win(i,:));
  
    
end
sum(win)
fprintf("%g, %g, %g, %g, %g, \n", win(end,:));