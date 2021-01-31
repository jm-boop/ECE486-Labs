function s = init_running_meanAN(M,blocksize)
%% Creates the structure for running mean value. The structure contains the
% places to find the running mean value of a sample. 

%Group one: John Bowen, Nate McNutt, Annalyse Nichols 
%ECE 486- Lab one
%intial struct filled with just zeros
%last: a buffer containing M-1 samples of the sample. 
%blocksize: length 
%samples: samples
%previous: running sum of the samples
%lastIndex: location
s = struct( 'M',M, ...
                        'blocksize',blocksize,...
                        'previous',0,...
                        'last', zeros(M-1,1),...
                        'samples',0,...
                        'lastIndex',1);
end