%%%%%testing%%%%%%%%%%%%test%%%%%%%%%%%%%%%%%%%%%%%%%%
init_running_mean.m
function s = init_running_mean(M,blocksize);
%intial struct filled with just zeros
s=struct('lastM',zeros(1,M),'block',zeros(1,blocksize),'blocksize',blocksize,'M',M'j',0);
%running mean function 
%%
calc_running_mean.m
function[y,s] = calc_running_mean(x,s)

%filling each range
for b = 1:s.blocksize
   s.block(b) = x(s.j+b)
end
for b = 1:s.blocksize
    last= s.lastM(mod(b+s.j-1,s.M)+1) = s.block(b);
    if b ==1
        s.y(b) = s.y(s.blocksize)+s.block(b)/s.M-last/s.M;
    end
end
s.j = s.j+s.blocksize;
y = s.y;

