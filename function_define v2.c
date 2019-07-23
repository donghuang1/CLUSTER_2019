#include "function.h"
#include <io.h>
#define PI 3.141592654 
#define m 100
#define n 30000
#define LAMBDA 500.0
#define RATIOOFEVEN 0.5
double U_Random();
double *randomExponential(double lambda,int len)  ;
void GetServerDistribution();
typedef struct node
{
	int size;
	int server;
	double time;
}RNODE,*pRNODE;
pRNODE prequest[n] = {NULL};  
int sizeofserver[m] = {0};
double aveinterval[m] = {0};
int pos = 0;
static int s_i;
int i,k,j;
double *gaussrand(void)
{
	static double c1[n];
	c1[0]=0;
    static double U, V;
    static int phase = 0;
    double Z;
    double num;
 	for(i=1;i<n;i++)
 	{
 		if(phase == 0)
    	{
        	U = rand() / (RAND_MAX + 1.0);
       		V = rand() / (RAND_MAX + 1.0);
        	Z = sqrt(-2.0 * log(U))* sin(2.0 * PI * V);
    	}
    	else
    	{
        	Z = sqrt(-2.0 * log(U)) * cos(2.0 * PI * V);
    	} 
    	phase = 1 - phase;  //m+z*sd  均值m  标准差sd
 //   	Z = (double) ((int)(Z + 0.05)*10) /10;
 //   	printf("%.1lf   ",Z);
 //   	num=RAND/2 + 20*Z;
    	c1[i]=  (double) ((int)((RAND/2 + 20*Z )*10)) /10;
 //   	printf("c[i]=""%lf ",c[i]);
    	if(c1[i]<0)
		{
			i=i-1;
			continue;	
		} 
    	
    	for(j=0;j<i;++j)
		{
			if(c1[i]==c1[j])//剔除重复时间
			{
				i=i-1;
			}
		}
	} 
	
	for(i=0;i<n-1;++i)//排序
	{
		for(j=i+1;j<n;++j)
		{
			if(c1[i]>c1[j])
			{
				num=c1[i];
				c1[i]=c1[j];
				c1[j]=num;
			}
		}
	}
	for(i=0;i<n;++i)
		printf("%.1lf ",c1[i]);
	printf("\n");
	printf("\n");  
    return c1;
	
 }
 
double Uniform(void)  
{  
//	srand((int)time(0));
//	return (m*rand()/(RAND_MAX+1.0)) ;
    static int seed;  
    static int flag = 0;  
      
    if (flag == 0)  
    {  
        flag = 1;  
//        srand(time(NULL));      // 设置随机数种子为当前时间的秒数  
        seed = rand();  
    }     
      
    seed = 2045 * seed + 1;  
    seed = seed - (seed / 1048576) * 1048576; 
    return ((seed) / 1048576.0);        
}  

void GetServerDistribution()
{
	int server;
	for(i=1;i<n;i++)
	{
		static int c2[n];
		server = 0;
		double tmp = Uniform();
//		printf("tmp=%lf ",tmp);
		if(tmp<RATIOOFEVEN)
		{
			double b= Uniform();	
//			printf("b=%lf  ",b);
			server = (int)(b*m);
			sizeofserver[server]++;		
		}
		else
		{
			double b = Uniform();
			server = (int)(b*2);
//			printf("server=%d  ",server);
			sizeofserver[m-server-1]++;		
		}
	}
	for(i=0;i<m;i++)
		printf("%d  ",sizeofserver[i]);
}
int* intensive(void)
{
	static int c2[n];
	c2[0]=0; 
	for(i=1;i<n;i++)
	{
		double a = Uniform() * pow(2,m);
//		printf("%lf  ",a);
		for(j=1;j<=m;j++)
		{
			if(((a - pow(2,j-1))>0 || fabs(a-pow(2,j-1))<0.000001) &&  (a - pow(2,j))<0)
			{
				c2[i] = j-1;
				sizeofserver[j-1]++;
//				printf("%d  ",c2[i]);
			}
			else if((a>0 || fabs(a)<0.000001) && a<1)
			{
				c2[i]=0;
				sizeofserver[0]++;
//				printf("%d  ",c2[i]);
				break;
			}				
		}		
	}
	printf("\n");
	for(i=0;i<m;i++)
		printf("%d  ",sizeofserver[i]);
	return c2;
}
//均匀分布 
int* uniform(void)
{
	printf("n=%d    m=%d\n",n,m);
	static int c3[n];
	c3[0]=0; 
	for(i=1;i<n;i++)
	{
		c3[i] = (int) (Uniform() * m );
//		printf("%d  ",c3[i]);
	}
	printf("\n");
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			if(c3[i]==j)
			sizeofserver[j]++;
		}
			
	}
	for(i=0;i<m;i++)
		printf("%d  ",sizeofserver[i]);
	return c3;
}
//泊松分布 
double* possion(int Lambda)  //均值和方差都为Lambda=n*p; 
{  
	static double c4[n];
	c4[0]=0;
	double num;
	srand((unsigned)time(NULL));  
	for(i=1;i<n;i++)
	{
		k = 0;  
    	long double p = 1.0;  
    	long double l = exp(-Lambda);  
        //printf("%1.5Lf\n", l);  
    	while(p>=l)  
    	{  
        	double v = U_Random();  
        	p *= v;  
        	k++;  
    	}  
    	c4[i]=(double) (k-1);
 //   	printf("%.1lf  ",c4[i]);
    	for(j=0;j<i;++j)
		{
			if(fabs(c4[i]-c4[j])<0.000001)//剔除重复时间
			{
				i=i-1;
			}
		} 	
	} 
	for(i=0;i<n-1;++i)//排序
	{
		for(j=i+1;j<n;++j)
		{
			if(c4[i]>c4[j])
			{
				num=c4[i];
				c4[i]=c4[j];
				c4[j]=num;
			}
		}
	}
	printf("\n"); 
	printf("\n"); 
	for(i=0;i<n;++i)
		printf("%.1lf ",c4[i]);
	printf("\n");
	printf("\n");   
    return c4;  
}  
  
double U_Random()  
{  
    double f;  
    f = (double)(rand() %100); 
//	printf("%lf  ",f); 
    return f/100;  
}  

double * exponent()  
{
	totalelements = 0;
	int*tmp = uniform();//GetServerDistribution();//intensive()
	double pv = 0.0;  
	double lambda[m]; //= {0.0001,0.0008,0.001,0.005};
	double *server0;
	//产生服从指数分布的lambda 
//	srand((int)time(0));
	for(i=0;i<m;i++)
	{
		pv = (double)(rand()%1000)/1000; 
	    while(pv == 0)  
	    {  
	        pv = (double)(rand() % 1000)/1000;  
	    }  
	    lambda[i] = (-1  / LAMBDA)*log(1-pv);
		for(j=0;j<i;j++)
		{
			if(fabs(lambda[i]-lambda[j])<0.000001)
			i--;
		} 
	}
	BubbleSort(lambda,m-1);
	printf("\n");
	for(i=0;i<m;i++)
		printf("%lf  ",lambda[i]);
	for(i=0;i<m;i++)
	{
		server0 = randomExponential(lambda[i],sizeofserver[i]);
	}
	double *server1 = (double*)malloc(totalelements*sizeof(double));	
	printf("totalelements=:%d\n",totalelements);
	for(i=0;i<m;i++)
		printf("%d   ,",sizeofserver[i]);
	FILE *fp;  
    fp=fopen("workload2.txt","a"); 
    int pos = fseek(fp,0,SEEK_END);  
//    fprintf(fp,"\n");
    for(i=0;i<m;i++)
    {
    	fprintf(fp,"%d	%d	%.2lf	%lf",i+1,sizeofserver[i],aveinterval[i],lambda[i]);
    	fprintf(fp,"\n");
	}
	
	fclose(fp);
	j=0;
	for(i=0;i<n;i++)
	{
		if(fabs(server0[i]) < 0.000001)
			continue;
		else
		{
			server1[j] = server0[i];
			j++;
		}
	}
	BubbleSort(server1,totalelements-1);
	printf("\n");	
//	for(i=0;i<totalelements;i++)
// 		printf("%lf  ",server1[i]);
	return server1;
} 
 
double *randomExponential(double lambda,int len)  
{
//	printf("lambda=%lf   len=%d  \n",lambda,len);
    double pv = 0.0;
    double tmp=0.0;
    double tmp1=0.0;
    int count = 0;
	double *arr= (double*)malloc(len*sizeof(double)); 
	static double arrallele[n];// = (double*)malloc(n*sizeof(double));
	srand((int)time(0));
	for(k=0;k<len;k++)
	{
		pv = (double)(rand()%10000)/10000;  
	    while(pv == 0)  
	    {  
	        pv = (double)(rand() % 10000)/10000;  
	    }  
	    arr[k] =(double)((int)(((-1  / lambda)*log(1-pv)+0.0005)*1000))/1000;//(double)((int)(((-1  / lambda)*log(1-pv)+0.05)*10))/10; (-1  / lambda)*log(1-pv);////// 
	    if(arr[k] > threshold)
	    	continue;
	    count++;
		arrallele[s_i]=arr[k];
		//去重
		for(j=0;j<s_i;j++)
		{
			if(fabs(arrallele[j] - arr[k])<0.000001)
			{
//				printf("in if!");
				k--;
				s_i--;
				count--;	
				break;
			}	
		}
		s_i++; 	
//		printf("%d ",s_i);
	}
	sizeofserver[pos] = count;
	pos++;
	totalelements+=count;
   	BubbleSort(arr,len-1);
   	printf("\n");
//    for(j=0;j<len;j++)
//		printf("%lf  ",arr[j]); 
//	printf("\n");
	if(count==0)
	{
		aveinterval[pos-1] = 0; 
		printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
	}
	else if(count==1)
	{
		aveinterval[pos-1] = arr[0]; 
		printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
	} 
	else
	{
		for(k=0;k<len;k++)
	   	{
//	   		if(arr[0]>threshold)
//	   		{
//	   			aveinterval[pos-1] = 0; 
//	   			printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
//				break;
//			}
//			if(arr[1]>threshold)
//	   		{
//	   			aveinterval[pos-1] = arr[0];
//				printf("aveinterval=%.1lf  ",aveinterval[pos-1]); 
//				break;
//			}
	   		if(fabs(arr[k]-threshold)<0.000001)
	   		{
	   			tmp = arr[k];
	   			tmp1 = arr[0];
	   			aveinterval[pos-1] = (tmp-tmp1)/(count-1); 
	   			printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
	   			break;
			}
	   		if(arr[k]>threshold)
	   		{
	   			tmp = arr[k-1];
	   			tmp1 = arr[0];
	   			aveinterval[pos-1] = (tmp-tmp1)/(count-1); 
	   			printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
	   			break;
			}
		}
		if(fabs(tmp)<0.000001)
		{
			tmp=arr[len-1];
			tmp1 = arr[0];
	   		aveinterval[pos-1] = (tmp-tmp1)/(count-1); 
	   		printf("aveinterval=%.1lf  ",aveinterval[pos-1]);
		}
	}
   	
	if(pos == m)
	{
		printf("s_i=%d\n",s_i); 
//		BubbleSort(arrallele,n-1);
//   		printf("\n");
   		for(j=0;j<n;j++)
//		printf("%lf  ",arrallele[j]); 		
		i=0;
		for(j=0;j<m;j++)
		{
			for(k=0;k<sizeofserver[j];k++)
			{
				pRNODE request = (pRNODE)malloc(sizeof(RNODE));
				prequest[i]=request;
				if(NULL==request)
					exit(-1);
				request->server = j;
				request->time = arrallele[i];
				i++;
			}
		}	
	}
	
		
//	printf("\n");
	free(arr);
    return arrallele;  					
}
int * GetServer()
{
//	static int c5[totalelements];
	int *c5 = (int*)malloc(totalelements*sizeof(int));
	pRNODE tmp = NULL;
	for(i=0;i<totalelements-1;i++)
	{
		for(j=0;j<totalelements-1-i;j++)
		{
			if(prequest[j]->time > prequest[j+1]->time)
			{
				tmp = prequest[j];
				prequest[j] = prequest[j+1];
				prequest[j+1] = tmp;
			}
		}
	}
//	printf("\n");
	for(i=0;i<totalelements;i++)
	{
		c5[i] = prequest[i]->server;
//		printf("%d  ",c5[i]);
	}
	return c5;
} 

double GetBeta()
{
	double ave = (double)totalelements / m;
	int maxcount = sizeofserver[0];
	for(i=1;i<m;i++)
	{
		if(sizeofserver[i] > maxcount)
		maxcount = sizeofserver[i];
	}
	double beta = ave / maxcount;
	beta = (double)((int)((beta+0.005)*100))/100;
	printf("ave=%lf,maxcount=%d,beta=%lf  \n",ave,maxcount,beta);
	return beta;
}

int * GetServerOrder()
{
	int *c7 =  (int*)malloc(m*sizeof(int));
	pRNODE parrrequset[m]; 
	for(i=0;i<m;i++)
	{
		pRNODE prequset = (pRNODE)malloc(sizeof(RNODE)*m);
		prequset->server = i;
		prequset->size = sizeofserver[i];
		parrrequset[i] = prequset;
	}
//	for(i=0;i<m;i++)
//	printf("%d : %d  ",parrrequset[i]->server,parrrequset[i]->size);
	pRNODE tmp = NULL;
	for(i=0;i<m-1;i++)
	{
		for(j=0;j<m-1-i;j++)
		{
			if(parrrequset[j]->size > parrrequset[j+1]->size)
			{
				tmp = parrrequset[j];
				parrrequset[j] = parrrequset[j+1];
				parrrequset[j+1] = tmp;
			}
		}
	}
//	printf("\n");
	for(i=0;i<m;i++)
	{
		c7[i] = parrrequset[i]->server;
//		printf("%d  ",c7[i]);
	}
	return c7;
	
}

void BubbleSort(double*arr,int len)
{
	double tmp =0;
	for(k=0;k<len;k++)
	{
		for(j=0;j<len-k;j++)
		{
			if(arr[j]>arr[j+1])
			{
				tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
}

void Get_Inputfile()
{
	//得到各个数据项的请求序列，文件第一行为请求的数量  注意修改输入文件 
	int size = 0;
	int circu = 0;
	int data = -1;
	int filelen[10];
	char *si,*ti;
	bool flag = false;
	char *test[DATASIZE]={};//{"data0.txt","data1.txt","data2.txt","data3.txt","data4.txt","data5.txt","data6.txt","data7.txt","data8.txt","data9.txt"};
	char str[DATASIZE+1][80000] = {};//为什么第一行用不了？ 
	FILE* inputfile = fopen("mergefile893124567a_s5.txt","r");//G:/trafficData20160510/multi_data/10/remove_series_s2.txt  G:/trafficData20160510/outputfile_mutirequest_6.txt    G:/trafficData20160510/multi_data/数据2/
//	FILE* outputfile[];//为什么文件数组有问题 
	FILE* outputfile;// = 
	if(inputfile == NULL)
	{
		printf("open file failed!");
		return;
	}
	for(i=0;i<DATASIZE;i++)
	{
		test[i] = (char*)malloc(sizeof(char)*8);
//		itoa(i,test[i],10);
		sprintf(test[i], "%d", i);
		strcat(test[i],".txt");
	
//		free(test[i]);
		puts(test[i]);
	}
//	for(i=0;i<DATASIZE;i++)
//	{
//		strcat(test[i+1],"data");
//		strcat(test[i+1],i+1);
//		strcat(test[i+1],".txt");
//		puts(test[i+1]);
//	 } 
//	FILE* out2 = fopen(test[2],"w");
//	fprintf(out2,"%s","vvvvvaaaabbbbb");
//	for(i=0;i<DATASIZE;i++)
//	{
//		outputfile[i] = fopen(test[i],"w");
//		printf("%s",test[i]);
//		fprintf(outputfile[i],"%s","aaaabbbbb");
//		if(outputfile[i] == NULL)
//		{
//			printf("open file failed!");
//			return;
//		}
//	}
	fscanf(inputfile,"%d ",&size);
	for(i=0;i<=size;i++)
    {
    	if(!flag)
    	{
    		flag = true;
    		continue;
		}
		char parr[] = "";
		fscanf(inputfile,"%s \n",&parr);
//		printf("%s ",parr);
//		puts(parr);
		char delims[] = ",";
		char *result = NULL;  
		result = strtok(parr,delims);
		while( result != NULL) 
		{
			if(circu==0)
			{
				si = result;
//				printf("aa%s",si);
			}
			else if(circu == 1)
				ti = result;
			else
			{
//				printf("%s,%s\n",si,ti); 
				sscanf(result, "%d", &data);
//				sscanf(si,ti,"%s,%s",si,ti);
//				printf("dddddd%d ",data);
				strcat(str[data],si);
				strcat(str[data],",");
				strcat(str[data],ti);
				strcat(str[data],"\n");
//				fseek(outputfile[data], 0, SEEK_END);
//				fprintf(outputfile[data],"%s",*si+","+*ti);
			} 
//        	printf( "result is \"%s\"\n", result );  
        	result = strtok( NULL, delims ); 
			circu++; 
   		}
		circu = 0;  
//		printf("\n");              
	}
	for(i=0;i<DATASIZE;i++)
	{
		outputfile = fopen(test[i],"w+");
		fprintf(outputfile,"%s",str[i+1]);
		fclose(outputfile);
	}
	for(i=0;i<DATASIZE;i++)
	{
		free(test[i]);
	}
	fclose(inputfile);
 } 
 
 
 void Merge_File(char* fname1,char* fname2,char* outputfile)//,char* outputfile
 {
// 	char* outputfile = "mergefile.txt";
//	if (!access(outputfile,0))
//	{
//		puts("outputfile exist");
//		remove(outputfile);
//	}
	if(strlen(fname1)==0)
	{
		puts("fname1=0");
		char buf2[32];
		FILE * fp = fopen(outputfile,"w");
 		FILE * fp2 = fopen(fname2,"r");
 		if(fp == NULL|| fp2 == NULL)
 		{
 			puts("open file failed!");
 			exit(1);
		 }
		char * s2 = fgets(buf2,32,fp2);
		while(s2!=NULL)
		{
			fprintf(fp,"%s",buf2);  /*从文件data1读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2);
		}
		fclose(fp);
		fclose(fp2);
		return;
	}
	if(strlen(fname2)==0)
	{
//		puts("fname2=0");
		char buf2[32];
		FILE * fp = fopen(outputfile,"w");
 		FILE * fp2 = fopen(fname1,"r");
 		if(fp == NULL || fp2 == NULL)
 		{
 			puts("open file failed!");
 			exit(1);
		 }
		char * s2 = fgets(buf2,32,fp2);
		while(s2!=NULL)
		{
//			puts(s2);
			fprintf(fp,"%s",buf2);  /*从文件data1读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2);
		}
		fclose(fp);
		fclose(fp2);
		return;
	}
	
 	char buf1[32];
 	char buf2[32];
	int len1,len2; 
 	int circu1 = 0;
 	int circu2 = 0;
// 	int s1,s2;
 	char *si1,*pti1,*si2,*pti2;
 	double ti1,ti2;
 	FILE * fp = fopen(outputfile,"w");
 	FILE * fp1 = fopen(fname1,"r");
 	FILE * fp2 = fopen(fname2,"r");
 	char parr1[32] = "";
 	char parr2[32] = "";
 	if(fp == NULL || fp1 == NULL || fp2 == NULL)
 	{
 		printf("xxopen file failed!\n");
 		exit(1);
	}
	char * s1 = fgets(buf1,32,fp1);
	char * s2 = fgets(buf2,32,fp2);
	while((s1 != NULL) && (s2 != NULL))
	{
		char delims[] = ",";
		char *result1 = NULL; 
		char *result2 = NULL;
		 
//		len1 = strlen(buf1);
//		buf1[len1-1] = '\0';  /*去掉换行符*/
//		parr1 = buf1;
//		puts(buf1);
		strcpy(parr1,buf1);
//		puts(parr1);
		result1 = strtok(buf1,delims);
//		printf("result1=%s ",result1);
		
		
		while( result1 != NULL)
		{
			if(circu1==0)
			{
				si1 = result1;
			}
			else if(circu1 == 1)
				pti1 = result1;
//			printf("ssss%s %d ",result1,circu1);
        	result1 = strtok( NULL, delims );
			circu1++;  
		} 
		
//		len2 = strlen(buf2);
//		buf2[len2-1] = '\0';  /*去掉换行符*/
//		puts(buf2);
//		parr2 = buf2;
		strcpy(parr2,buf2);
		result2 = strtok(buf2,delims);
//			printf("result2=%s ",result2);
		while( result2 != NULL)
		{
			if(circu2==0)
			{
				si2 = result2;
			}
			else if(circu2 == 1)
				pti2 = result2;
//			printf("aaaa%s ",result2);
        	result2 = strtok( NULL, delims );
			circu2++;  
		}
		
		sscanf(pti1,"%lf",&ti1); 
		sscanf(pti2,"%lf",&ti2); 
//		printf("%lf,%lf,%lf\n",ti1,ti2,fabs(ti1-ti2));
		if(fabs(ti1-ti2) < 0.00001)
		{
			fprintf(fp,"%s",parr1);  /*从文件data1读的数据写入文件data*/
			s1 = fgets(buf1,32,fp1);
			s2 = fgets(buf2,32,fp2);
		}
		else if(ti1<ti2 && fabs(ti1-ti2)>0.00001)
		{
			fprintf(fp,"%s",parr1);  /*从文件data1读的数据写入文件data*/
			s1 = fgets(buf1,32,fp1); /*从文件data1读下一个数据*/
//			buf2 = parr2;
			strcpy(buf2,parr2);
		}
		else
		{
			fprintf(fp,"%s",parr2);  /*从文件data2读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2); /*从文件data2读下一个数据*/
//			buf1 = parr1;
			strcpy(buf1,parr1);
//			puts(buf1);
		}
		circu1 = 0;
		circu2 = 0;
	}
	while(s1 != NULL)  /*文件data1的剩余数据写入文件data*/
	{
		fprintf(fp,"%s",buf1);  /*从文件data1读的数据写入文件data*/
		s1 = fgets(buf1,32,fp1);
//		s1=fscanf(fp1,"%s",&parr1); /*从文件data1读下一个数据*/
	}
	while( s2!= NULL) /*文件data2的剩余数据写入文件data*/
	{
		fprintf(fp,"%s",buf2);  /*从文件data2读的数据写入文件data*/
		s2 = fgets(buf2,32,fp2);
//		s2=fscanf(fp2,"%s",&parr2); /*从文件data2读下一个数据*/
	}
//	flushall();
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	return;	
 }

void Merge_File2(char fname1[],char fname2[],char *outputfile,int * arr)//,char* outputfile
 {
// 	char* outputfile = "mergefile.txt";
//	if (!access(outputfile,0))
//	{
//		puts("outputfile exist");
//		remove(outputfile);
//	}
	//解析文件名
	char str1[4];
	char str2[4];
	sprintf(str1,"%d",arr[0]);
	sprintf(str2,"%d",arr[1]);
//	puts(fname1);
//	puts(fname2);
////	if(strlen(fname1)==0) str1=" ";
////	if(strlen(fname2)==0) str2 = " ";
//	char str3[32];// = fname1;
//	char str4[32];// = fname2;
//	strcpy(str3,fname1);
//	str1 = strtok(str3,".");
//	
//	strcpy(str4,fname2);
//	str2 = strtok(str4,".");
//	
////	strcpy(str1,str3);
////	strcpy(str2,str4);
// 
//	printf("str1=%s		str2=%s	\n",str1,str2);
	
		
	if(strlen(fname1)==0)
	{
		puts("fname1=0");
		char buf2[32];
		FILE * fp = fopen(outputfile,"w");
 		FILE * fp2 = fopen(fname2,"r");
 		if(fp == NULL|| fp2 == NULL)
 		{
 			puts("open file failed!");
 			exit(1);
		 }
		char * s2 = fgets(buf2,32,fp2);
		while(s2!=NULL)
		{
//			buf2[strlen(buf2)-1] = '\0';
//			strcat(buf2,",");
//			strcat(buf2,str2);
//			strcat(buf2,"\n");
			fprintf(fp,"%s",buf2);  /*从文件data1读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2);
		}
		fclose(fp);
		fclose(fp2);
		return;
	}
	if(strlen(fname2)==0)
	{
//		puts("fname2=0");
		char buf2[32];
		FILE * fp = fopen(outputfile,"w");
 		FILE * fp2 = fopen(fname1,"r");
 		if(fp == NULL || fp2 == NULL)
 		{
 			puts("open file failed!");
 			exit(1);
		 }
		char * s2 = fgets(buf2,32,fp2);
		while(s2!=NULL)
		{
//			puts(s2);
//			buf2[strlen(buf2)-1] = '\0';
//			strcat(buf2,",");
//			strcat(buf2,str1);
//			strcat(buf2,"\n");
			fprintf(fp,"%s",buf2); /*从文件data1读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2);
		}
		fclose(fp);
		fclose(fp2);
		return;
	}
	
 	char buf1[32];
 	char buf2[32];
	int len1,len2; 
 	int circu1 = 0;
 	int circu2 = 0;
// 	int s1,s2;
 	char *si1,*pti1,*si2,*pti2;
 	double ti1,ti2;
 	FILE * fp = fopen(outputfile,"w");
 	FILE * fp1 = fopen(fname1,"r");
 	FILE * fp2 = fopen(fname2,"r");
 	
 	char parr1[32] = "";
 	char parr2[32] = "";
 	if(fp == NULL || fp1 == NULL || fp2 == NULL)
 	{
 		printf("xxopen file failed!\n");
 		exit(1);
	}
	char * s1 = fgets(buf1,32,fp1);
	char * s2 = fgets(buf2,32,fp2);
	while((s1 != NULL) && (s2 != NULL))
	{
		char delims[] = ",";
		char *result1 = NULL; 
		char *result2 = NULL;
		 
//		len1 = strlen(buf1);
//		buf1[len1-1] = '\0';  /*去掉换行符*/
//		parr1 = buf1;
//		puts(buf1);
		strcpy(parr1,buf1);
//		puts(parr1);
		result1 = strtok(buf1,delims);
//		printf("result1=%s ",result1);
		
		
		while( result1 != NULL)
		{
			if(circu1==0)
			{
				si1 = result1;
			}
			else if(circu1 == 1)
				pti1 = result1;
//			printf("ssss%s %d ",result1,circu1);
        	result1 = strtok( NULL, delims );
			circu1++;  
		} 
		
//		len2 = strlen(buf2);
//		buf2[len2-1] = '\0';  /*去掉换行符*/
//		puts(buf2);
//		parr2 = buf2;
		strcpy(parr2,buf2);
		result2 = strtok(buf2,delims);
//			printf("result2=%s ",result2);
		while( result2 != NULL)
		{
			if(circu2==0)
			{
				si2 = result2;
			}
			else if(circu2 == 1)
				pti2 = result2;
//			printf("aaaa%s ",result2);
        	result2 = strtok( NULL, delims );
			circu2++;  
		}
		
		sscanf(pti1,"%lf",&ti1); 
		sscanf(pti2,"%lf",&ti2); 
//		printf("%lf,%lf,%lf\n",ti1,ti2,fabs(ti1-ti2));
		if(fabs(ti1-ti2) < 0.00001)
		{
			parr1[strlen(parr1)-1] = '\0';//去掉字符串末尾的'\n' 
			strcat(parr1,",");
			strcat(parr1,str1);
			strcat(parr1,",");
			strcat(parr1,str2);
			strcat(parr1,"\n");
//			puts(parr1); 
			fprintf(fp,"%s",parr1);  /*从文件data1读的数据写入文件data*/
			s1 = fgets(buf1,32,fp1);
			s2 = fgets(buf2,32,fp2);
		}
		else if(ti1<ti2 && fabs(ti1-ti2)>0.00001)
		{
			parr1[strlen(parr1)-1] = '\0';
			strcat(parr1,",");
			strcat(parr1,str1);
			strcat(parr1,"\n");
			fprintf(fp,"%s",parr1);  /*从文件data1读的数据写入文件data*/
			s1 = fgets(buf1,32,fp1); /*从文件data1读下一个数据*/
//			buf2 = parr2;
			strcpy(buf2,parr2);
		}
		else
		{
			parr2[strlen(parr2)-1] = '\0';
			strcat(parr2,",");
			strcat(parr2,str2);
			strcat(parr2,"\n");
			fprintf(fp,"%s",parr2);/*从文件data2读的数据写入文件data*/
			s2 = fgets(buf2,32,fp2); /*从文件data2读下一个数据*/
//			buf1 = parr1;
			strcpy(buf1,parr1);
//			puts(buf1);
		}
		circu1 = 0;
		circu2 = 0;
	}
	while(s1 != NULL)  /*文件data1的剩余数据写入文件data*/
	{
		parr1[strlen(parr1)-1] = '\0';
		strcat(buf1,",");
		strcat(buf1,str1);
		strcat(parr1,"\n");
		fprintf(fp,"%s",buf1);  /*从文件data1读的数据写入文件data*/
		s1 = fgets(buf1,32,fp1);
//		s1=fscanf(fp1,"%s",&parr1); /*从文件data1读下一个数据*/
	}
	while( s2!= NULL) /*文件data2的剩余数据写入文件data*/
	{
		parr2[strlen(parr2)-1] = '\0';
		strcat(buf2,",");
		strcat(buf2,str2);
		strcat(parr2,"\n");
		fprintf(fp,"%s",buf2);/*从文件data2读的数据写入文件data*/
		s2 = fgets(buf2,32,fp2);
//		s2=fscanf(fp2,"%s",&parr2); /*从文件data2读下一个数据*/
	}
//	flushall();
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	return;	
 }
 
// void getCoExistence(char fname1[],char *outputfile1,char *outputfile2,char *outputfile)//,char* outputfile
// {
// 	FILE * fp = fopen(outputfile,"w");
// 	FILE * fp3 = fopen(fname1,"r");
// 	int q;
// 	char parr1[32] = "";
// 	char buf1[32];
// 	if(fp == NULL || fp3 == NULL)
// 	{
// 		printf("xopen file failed!\n");
// 		exit(1);
//	}
//	char * s1 = fgets(buf1,32,fp3);
//	while(s1 != NULL) 
//	{
//		q=0;
//		char delims[] = ",";
//		char *result1 = NULL; 
//		strcpy(parr1,buf1);
//		puts(parr1);
////		puts(parr1);
//		result1 = strtok(buf1,delims);
////		printf("result1=%s ",result1);
//		while( result1 != NULL)
//		{
//			q++;
//        	result1 = strtok( NULL, delims );
//		}
//		if(q==4)
//		{
//			fprintf(fp,"%s",parr1); 	
//		}
//		s1 = fgets(buf1,32,fp3); 
//	}
//	fclose(fp);
//	fclose(fp1);
//	puts("ssssssssssssssssssssssssssssssssssss");
//	return;
//}

void getCoExistence(char fname1[],char *outputfile1,char *outputfile2,char *outputfile,int* arr)//,char* outputfile
 {
 	FILE * fp = fopen(outputfile,"w");
 	FILE * fp1 = fopen(outputfile1,"w");
 	FILE * fp2 = fopen(outputfile2,"w");
 	FILE * fp3 = fopen(fname1,"r");
 	int q;
 	char parr1[32] = "";
 	char buf1[32];
 	if(fp == NULL || fp3 == NULL || fp1 == NULL || fp2 == NULL)
 	{
 		printf("xopen file failed!\n");
 		exit(1);
	}
	char * s1 = fgets(buf1,32,fp3);
	while(s1 != NULL) 
	{
		q=0;
		char delims[] = ",";
		char *result1 = NULL; 
		char * result_tmp=NULL;
		strcpy(parr1,buf1);
//		puts(parr1);
//		puts(parr1);
		result1 = strtok(buf1,delims);
//		printf("result1=%s ",result1);
		while( result1 != NULL)
		{
			q++;
			result_tmp = result1;
        	result1 = strtok( NULL, delims );
		}
//		char str1[3];
//		strcpy(str1,result1);
		int item;
		sscanf(result_tmp,"%d",&item);
//		puts(parr1);
//		printf("q=%d	item=%d	arr[0]=%d	arr[1]=%d\n",q,item,arr[0],arr[1]);
		if((q==3 && item==arr[0]) || q==4) fprintf(fp1,"%s",parr1);
		if((q==3 && item==arr[1]) || q==4)  fprintf(fp2,"%s",parr1);
		if(q==4)
		{
			fprintf(fp,"%s",parr1); 	
		}
		s1 = fgets(buf1,32,fp3); 
	}
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
//	puts("ssssssssssssssssssssssssssssssssssss");
	return;
}


 
