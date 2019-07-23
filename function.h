#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include <stdio.h>
double *possion(int Lambda); //the request obey the Possion distribution
double* gaussrand(void);  //the request obey the Gaussian distribution 
double Uniform(void);
int* intensive(void); //there exist intensive server
int* uniform(void);   //the request is uniform for server
double * exponent();
int * GetServer();
double GetBeta();
void BubbleSort(double*arr,int len);
void Merge_File(char* fname1,char* fname2,char* outputfile);
// void getCoExistence(char fname1[],char *outputfile);//,char* outputfile
void getCoExistence(char fname1[],char *outputfile1,char *outputfile2,char *outputfile,int* arr);//,char* outputfile
void Merge_File2(char fname1[],char fname2[],char *outputfile,int * arr);//,char* outputfile
int * GetServerOrder();
void Get_Inputfile();
int totalelements;
//const int m = 4;
//const int n = 5; 
#define m 100
//#define n 4000//存放请求的总个数
#define RAND 130 //请求随机数的范围
#define  threshold 1900.0
#define u 1.0
#define tr 5.0
#define dt tr/u
#define DATASIZE 10
#define CORTHRE 0.5
#define THETA 1.0




