function s = init_running_mean(M,blocksize)
%% Creates the structure for running mean value. The structure contains the
% places to find the running mean value of a sample. 

%Group one: John Bowen, Nate McNutt, Annalyse Nichols 
%ECE 486- Lab one
%intial struct filled with just zeros
%last: a buffer containing M-1 samples of the sample. 
%blocksize: length 
s = struct( 'M',M, ...
            'blocksize',blocksize,...
            'last', zeros(1,M-1));
end