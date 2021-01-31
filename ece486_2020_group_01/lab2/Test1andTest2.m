%% Test One and two
% This script is our first test for the biquad filter in C.
% We are using the numbers from the homework and some of our own to get 
% a third stage. The C_output_* are copied from our C output. 

% the input x[n] only has values for the first 4 elements
 N = [3, 1.5, -1.8,0.2];
 % I need to add 16 zeros to x.
 zero16 = zeros(1,16);
 x = [N,zero16];
 % Here the blocksize is 20

 
 % Get filter output with with filter with first set of coeff and
 % then feed that output into a second filter call with the second set of
 % coeff
 Filter_coe_1A = [1, -1.5371, 0.9025];
 Filter_coe_1B = [1, 1.618, 1];

 
 Filter_coe_2A = [1, 0, -0.8100];
 Filter_coe_2B = [1, -0.618, 1];
 
 Filter_coe_3A = [1, -1.34, 0.67 ];
 Filter_coe_3B = [1, -0.789, 1.67 ];
 
 % three stages for the filter
 filter1_y=filter(Filter_coe_1B,Filter_coe_1A,x);
 filter2_y = filter(Filter_coe_2B,Filter_coe_2A,filter1_y);
 filter3_y = filter(Filter_coe_3B,Filter_coe_3A, filter2_y);
 
 
 %% C Variables
 % This is the part where we hardcode the output from the C file
 
C_output_stage_one = [3
10.9653
17.774261
16.212234
7.402154
-3.053691
-11.374271
-14.727437
-12.372263
-5.725893
2.364697
8.802395
11.396022
9.572664
4.429233
-1.831156
-6.812052
-8.818186
-7.406556
-3.426204
];

C_output_stage_two=[3
9.1113
16.427708
23.573193
28.463696
27.678295
20.970657
11.667588
2.341254
-3.356525
-4.572548
-1.103666
4.617076
10.438349
13.649179
13.459306
9.80467
4.462543
-0.827187
-4.052478
     ];

 
 C_output_stage_three =[3
10.764299
26.663055
54.344017
92.255447
131.799484
161.46701
169.404633
146.97583
97.7276
34.466827
-22.393269
-55.248039
-54.07653
-22.322533
26.441227
72.366753
98.459518
95.4757
66.022179
     
 ];
% transpose for the next section
C_output_stage_one = C_output_stage_one.';
C_output_stage_two = C_output_stage_two.';
C_output_stage_three = C_output_stage_three.';
 
 %Then we compare the values using the compare_values function
 
 %% Stage one
 % Compare function output with actual averages with x places of
% decimal accuracy
 decimal_acc = 3;
 
% we looking at each index and seeing if the values match
compare_values(filter1_y,C_output_stage_one,decimal_acc);
%% Stage 1 not rounded
stem(abs(C_output_stage_one-filter1_y));
title("Stage one filter coefficient comparion, unrounded");
xlabel("coefficient");
ylabel('absolute difference');
%ylim([-125 0]);

%% Stage 1 Rounded
stem(round(abs(C_output_stage_one-filter1_y),decimal_acc));
title("Stage one filter coefficient comparion, rounded to 4 places");
xlabel("coefficient");
ylabel('absolute difference');

%% Stage 2 not rounded
stem(abs(C_output_stage_two-filter2_y));
title("Stage two filter coefficient comparion, unrounded");
xlabel("coefficient");
ylabel('absolute difference');
%% Stage 2 rounded
stem(round(abs(C_output_stage_two-filter2_y),decimal_acc));
title("Stage two filter coefficient comparion, rounded to 4 places");
xlabel("coefficient");
ylabel('absolute difference');

%% stage 3 not rounded
compare_values(filter3_y,C_output_stage_three, decimal_acc);
 stem(abs(C_output_stage_three-filter3_y));
title("Stage three filter coefficient comparion, unrounded");
xlabel("coefficient");
ylabel('absolute difference');

%% Stage 3 rounded
stem(round(abs(C_output_stage_two-filter2_y),decimal_acc));
title("Stage three filter coefficient comparion, rounded to 3 places");
xlabel("coefficient");
ylabel('absolute difference');