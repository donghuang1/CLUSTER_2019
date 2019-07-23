Data Caching Algorithm With Package Strategy(DP_Greedy)
INPUT: the request sequence file(si ti di) and the file format is shown as mergefile893124567a_s5.txt, and the package strategy file and the file format is shown as package110_s4.txt where a line with two number(data item) represents the two data items' jaccard similarity is greater than the threshold. 
OUTPUT: the DP_Greedy algorithm cost.
The project mainly contains 3 files, that is main v4.c, function_define v2.c, function.h.

1¡¢function.h mainly contains the head file, some macro definitions of variables and declaration of some functions used by the project, in which the meaning of some important variables and functions is shown as follows:
	m: the number of server;
	n: the number of request;
	u: the cost of caching per time unit(¦Ì);
	tr: the cost of transfer between each server pair(¦Ë);
	DATASIZE: the number of data items;
	THETA: the discount factor;
	double *possion(int Lambda): generate the request sequences subject to poisson distribution;
	double* gaussrand(void),double Uniform(void), int* intensive(void), int* uniform(void), double * exponent(): generate the request sequences subject to various distributions(not be used in this project);
	void getCoExistence(char fname1[],char *outputfile1,char *outputfile2,char *outputfile,int* arr): Given the inputfile fname1, to get the request sequence with two data items coexistence and request sequence with data item d1 and request sequence with data item d2 and store in different file respectly.
	void Merge_File(char* fname1,char* fname2,char* outputfile): Given request sequence fname1 and fname2, merging two sequence to one based on the request's ti; 
	void Merge_File2(char fname1[],char fname2[],char *outputfile,int * arr): Given request sequence fname1 and fname2, merging two sequence to one based on the request's ti. the array arr stores the two merge data items, and the difference between function Merge_File2 and Merge_File is that the function Merge_File2 adds the data items at the end of each line.

2¡¢function_define v2.c mainly contains the some important  auxiliary functions' definetion .
	void Get_Inputfile(): In this function, the input file should be given and the input file is request sequence(si,ti,di) and the file format is as the file mergefile893124567a_s5.txt shown. the output of this function is to get each data item's request sequence and store in its file;
		input: mergefile893124567a_s5.txt;
		output: 0.txt, 1.txt, 2.txt, 3.txt, 4.txt, 5.txt, 6.txt, 7.txt, 8.txt, 9.txt;
	void Merge_File(char* fname1,char* fname2,char* outputfile): Given request sequence fname1 and fname2, merging two sequence to one based on the request's ti; 
		input: 8.txt, 1.txt
		output:21.txt
	void Merge_File2(char fname1[],char fname2[],char *outputfile,int * arr): Given request sequence fname1 and fname2, merging two sequence to one based on the request's ti. the array arr stores the two merge data items, and the difference between function Merge_File2 and Merge_File is that the function Merge_File2 adds the data items at the end of each line.
		input: 8.txt, 1.txt
		output: 51.txt
	void getCoExistence(char fname1[],char *outputfile1,char *outputfile2,char *outputfile,int* arr): Given the inputfile fname1(51.txt), to get the request sequence with two data items coexistence(52.txt) and request sequence with data item d1(53.txt) and request sequence with data item d2(54.txt) and store in different file respectly.
		input: 51.txt
		output: 52.txt, 53.txt, 54.txt

3¡¢main v5.c£ºThe main algorithm implementation is shown in this file. Some functions' role as follows:
	void Init(): Mainly do some initialization work;
	int * FormServerSeq(): Randomly generate server sequence(not be used in this project);
	double * FormRequestSeq(): Randomly generate request sequence(not be used in this project);
	int PivotIndex(int index): Given the request index, find the pivot index k;
	void ComputeBi(): Compute all requests' Bi, and store in array Bi;
	double ComputeOff_line(int index): Given the request index, compute the off line cost using the optimal algorithm;
	void Get_doublelinked_list(char *path): Given the request sequence, storing the request by the double linked list form;
	void compute_cost_correlation(double *cost_cor,double *cost): The unpacked(²»²ð°üµÄ) data caching algorithm that as long as the jaccard similarity of two data items is greater than the threshold, whether it appears alone or together, it is served by the data package.
	void compute_cost_DPgreedy(double *cost_cor,double *cost): The proposed DP_Greedy algorithm, the two input array cost_cor and cost is the cost of function compute_cost_correlation and  ComputeOff_line and for the purpose of comparison.
	double computeGreedy(int index): A auxiliary function of DP_Greedy that when a request access one of the package data item that should be served by greedy strategy;

	