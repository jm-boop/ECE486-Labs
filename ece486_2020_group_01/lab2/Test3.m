%% Test Three
% This script is nearly identically to Test1andTest2.m expect that we are
% only looking at the thrid stage coef for 2 calls to calc_biquad
% function. We mimic this in matlab by concatating our input with itself. 


% the input x[n] only has values for the first 4 elements
 N = [3, 1.5, -1.8,0.2];
 % I need to add 16 zeros to x.
 ones7 = ones(1,7);
 x = [N,ones7];
 x = [x,x];
 % Here the blocksize is 20
 %x = (1/max(cascade)) *x;
 
 % Get filter output with with filter with first set of coeff and
 % then feed that output into a second filter call with the second set of
 % coeff
 Filter_coe_1A = [1, -1.5371, 0.9025];
 Filter_coe_1B = [1, 1.618, 1];

 
 Filter_coe_2A = [1, 0, -0.8100];
 Filter_coe_2B = [1, -0.618, 1];
 
 Filter_coe_3A = [1, -1.34, 0.67 ];
 Filter_coe_3B = [1, -0.789, 1.67 ];
 
 filter1_y=filter(Filter_coe_1B,Filter_coe_1A,x);
 filter2_y = filter(Filter_coe_2B,Filter_coe_2A,filter1_y);
 filter3_y = filter(Filter_coe_3B,Filter_coe_3A, filter2_y);
 %%%
 
 filter4_y = filter(Filter_coe_1B,Filter_coe_1A,x);
 filter5_y = filter(Filter_coe_2B,Filter_coe_2A,filter4_y);
 filter6_y = filter(Filter_coe_3B,Filter_coe_3A, filter5_y);
 
 filter6C_version = filter6_y(end-10:end);
 
 %% From the C output
 % This is the part where we hardcode the output from the C file
 
C_output_stage_one = [
];
C_output_stage_two=[];

 
 C_output_stage_three =[177.182404
198.364471
244.157776
306.87204
368.840332
407.955017
408.889832
369.296204
300.456268
227.846588
179.762787
   ];
C_output_stage_one = C_output_stage_one.';
C_output_stage_two = C_output_stage_two.';
C_output_stage_three = C_output_stage_three.';
 
 %%%% Then we compare the values using the compare values function
 
% Compare function output with actual averages with x places of
% decimal accuracy
 decimal_acc = 3;
 

%% stage 3 not rounded (6)
compare_values(filter6C_version,C_output_stage_three, 3);
 stem(abs(C_output_stage_three-filter6C_version));
title("Stage three filter coefficient comparion, unrounded");
xlabel("coefficient");
ylabel('absolute difference');

%% Stage 3 rounded (6)
stem(round(abs(C_output_stage_three-filter6C_version),3));
title("Stage three filter coefficient comparion, rounded to 3 places");
xlabel("coefficient");
ylabel('absolute difference');