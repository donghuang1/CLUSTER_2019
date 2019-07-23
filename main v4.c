/*
1、寻找频繁项集 
2、对频繁项集进行打包，考虑最简单的情况，数据项不会重复打包，参与打包的数据项数不确定 
**/
#include "function.h"
#include <float.h>
#define countinterval 100
typedef struct node
{
	int id;
	int serveway[2];//第一个位置存放服务的类型 1代表cache 2代表transfer 0代表load  第二个位置存放从哪个请求来服务 
	double time;//存放请求发生的时间
	struct node * pfront;//存放同一个服务器上该请求的前一个请求
	struct node * pnext;//存放同一个服务器上该请求的后一个请求
	struct node * pcover[m];//存放cover该请求的线段的前一个请求
	int nodesize;
	int server;
}NODE,*PNODE; 
void Init();
int * FormServerSeq();
double * FormRequestSeq();
int PivotIndex(int index);
double ComputeBi(int index); 
double ComputeOff_line(int index);
double ComputeOn_line_default(int index);
double ComputeOn_line_priority(int index);
double ComputeOn_line_random(int index);
double ComputeOn_line_balance(int index);
void compute_cost_correlation(double *cost_cor,double* cost);
//void compute_cost_DPgreedy(double *cost_cor,double *cost);
int ** Get_correlationmatrix(char* inputpath);
void GetAveReplicas(int*replicas);
void Get_doublelinked_list(char *path);
void compute_cost_DPgreedy(double *cost_cor,double *cost); 
double computeGreedy(int index);
int * GetServerSize_except();//得到除开小cache的点的请求个数 
int *pserversize;
int i,j,k,p,r; //si,ti分别代表请求发生的服务器号和时间，i,j,k循环控制变量，num在排序时作中间变量
int n;
double *Bi;
double num;
double off_line_cost;
double on_line_cost;
double on_line_cost_balance;
double on_line_cost_priority;
double on_line_cost_random;
int numoftransfer;
int numoftransfer_balance; 
int numoftransfer_priority; 
int numoftransfer_off;
double ratiooftransfer_off; 
double ratiooftransfer;
int *replicas_off;
int *replicas_on;
int *replicas_on_balance;
int *replicas_on_priority;
int *replicas_on_random;
int *serverorder;
int arrsize; 
//int totalelements;
double * arroffline;
double * arronline;
double *gama_online_timely;
double *a;//存放随机产生的请求节点发生的时间
int *b;//存放随机产生的请求发生的服务器编号
double beta	;
int windowsize;
int slidelen;
PNODE *pindex;//存放节点的地址
PNODE pLast[m];//存放每个服务器最后一个请求
PNODE pinvalid;
int main(void)
{
	Get_Inputfile();
//	int arr[2] = {0,8};
//	Merge_File2("0.txt","8.txt","60.txt",arr);
	
//	getCoExistence("51.txt","52.txt","53.txt","54.txt",arr);
	//形成数据相关性矩阵
	double cost[DATASIZE] = {0};
	double cost_cor[DATASIZE] = {0};
	double cost_DPGreedy[DATASIZE]= {0};
	double total_cost=0;
	double total_cost_cor = 0;
	double total_cost_DPGreedy = 0;
	float correlation[DATASIZE][DATASIZE] = {};
//	srand((unsigned)time(NULL));
	for(i=0;i<DATASIZE;i++)
	{
	 	for(j=0;j<=i;j++)
	 	{
	 		if(i==j) correlation[i][j]=1.0;
	 		else
	 		{
				correlation[i][j]=(float)rand()/RAND_MAX;
				correlation[j][i] = correlation[i][j];  
			}
//			printf("%f\n",correlation[i][j]);
		}	
	}
	//计算没有相关性时每个数据的代价
	for(p=0;p<DATASIZE;p++)
	{
		Init();	
		//得到文件名 
		char path[] = "";
		itoa(p,path,10);
		strcat(path,".txt");//i.txt	
//		puts(path);
		Get_doublelinked_list(path);
		cost[p] = ComputeOff_line(n-1); 
		totalelements+=n;
//		printf("totalelements=%d\n",totalelements);
//		printf("cost=%lf  ",cost[p]);
	}
	for(i=0;i<DATASIZE;i++)
	{
//		printf("%lf  ",cost[i]);
		total_cost+=cost[i];
		cost_cor[i] = cost[i];
		cost_DPGreedy[i] = cost[i];
	}
		//计算数据存在相关性时的代价
	compute_cost_correlation(cost_cor,cost);	
	
	compute_cost_DPgreedy(cost_DPGreedy,cost);
//	printf("dpgreedy:");
	for(i=0;i<DATASIZE;i++)
	{
		printf("%lf  ",cost_DPGreedy[i]);
		total_cost_DPGreedy+=cost_DPGreedy[i];
	}
	
	
	printf("\npack:"); 
	for(i=0;i<DATASIZE;i++)
	{
		printf("%lf  ",cost_cor[i]);
		total_cost_cor+=cost_cor[i];
	}
	
	
	
	printf("\ntotalelements=%d\n",totalelements);
	printf("\ncost_base=%lf,  cost_pack=%lf,  cost_dpgreedy=%lf\n",total_cost,total_cost_cor,total_cost_DPGreedy);
	
//	double cost_tmp[3];
//	char *test[3]={"G:/trafficData20160510/multi_data/10/extract0.txt","G:/trafficData20160510/multi_data/10/extract8.txt","G:/trafficData20160510/multi_data/10/extract18.txt"};
//	for(p=0;p<3;p++)
//	{
//		Init();	
//		Get_doublelinked_list(test[p]);
//		cost_tmp[p] = ComputeOff_line(n-1);
////		printf("cost_tmp=%lf\n",cost_tmp[p]); 
//	
//	}
//	double cost_tmp_sum;
//	cost_tmp_sum = cost_tmp[0]+cost_tmp[1]+1.6*cost_tmp[2];



//	printf("cost_tmp=%lf\n",cost_tmp_sum); 
//	ratiooftransfer_off = (double)numoftransfer_off / n *100;
//	printf("n=%d,ratiooftransfer_off=%lf",n,ratiooftransfer_off);


//	FILE* fp1 = fopen("result10_3.txt","a");
//	fseek(fp1,0,SEEK_END);
//	fprintf(fp1,"\n");
//	fprintf(fp1,"%.1lf  %.1lf	%.1lf		%lf		%lf		%lf",u,tr,tr/u,total_cost/totalelements,total_cost_cor/totalelements,total_cost_DPGreedy/totalelements);
//	fclose(fp1);
	
	
	
//	printf("%d  ",PivotIndex(15)); 
//	printf("%d  ",ComputBi(4));
//	pserversize = GetServerSize_except();
//	for(i=0;i<arrsize;i++)
//	printf("aa%d",pserversize[i]);
//	off_line_cost = ComputeOff_line(n-1); 
//	on_line_cost = ComputeOn_line_default(n-1);
//	on_line_cost_balance = ComputeOn_line_balance(n-1);
//	on_line_cost_priority = ComputeOn_line_priority(n-1);
//	on_line_cost_random = ComputeOn_line_random(n-1);
//	double ratio = (double)((int)((on_line_cost/off_line_cost+0.005)*100))/100;
//	ratiooftransfer = (double)numoftransfer / n *100;
//	ratiooftransfer_off = (double)numoftransfer_off / n *100;
//	double eachcost = (double)((int)((off_line_cost/n+0.005)*100))/100;
//	
//	GetAveReplicas(replicas_off);
//	FILE *fp;  
//    fp=fopen("Result2.txt","a"); 
//    int pos = fseek(fp,0,SEEK_END);  
//    fprintf(fp,"\n");
//    
//	fprintf(fp,"%d	%d	%.2lf	%.0lf	%.1lf	%.1lf	%.1lf			%.1lf			%.1lf			%.1lf			%.1lf		%.2lf	%.2lf	%.2lf	%.2lf",
//				m,	n,	beta,threshold,  tr,	 u,	off_line_cost,on_line_cost,on_line_cost_balance,on_line_cost_priority,on_line_cost_random,ratio,ratiooftransfer,ratiooftransfer_off,eachcost);
//	fclose(fp);
//	FILE *fp1;  
//    fp1=fopen("replicas3.txt","a"); 
//    int pos2 = fseek(fp1,0,SEEK_END);  
//    fprintf(fp1,"\n");
//    fprintf(fp1,"m=%d,n=%d,beta=%.2lf,tr=%.1lf,u=%.1lf\n",m,n,beta,tr,u);
//    fprintf(fp1,"replicas_offline:\n");
//    for(i=0;i<n;i++)
//    	fprintf(fp1,"%d  ",replicas_off[i]);
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"replicas_online_default:\n");
//    for(i=0;i<n;i++)
//    	fprintf(fp1,"%d  ",replicas_on[i]);
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"replicas_online_balance:\n");
//     for(i=0;i<n;i++)
//    	fprintf(fp1,"%d  ",replicas_on_balance[i]);
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"replicas_online_priority:\n");
//    for(i=0;i<n;i++)
//    	fprintf(fp1,"%d  ",replicas_on_priority[i]);
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"replicas_online_random:\n");
//    for(i=0;i<n;i++)
//    	fprintf(fp1,"%d  ",replicas_on_random[i]);
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fprintf(fp1,"\n");
//    fclose(fp1); 
    
//    FILE *fp2;  
//    fp2=fopen("gama_timely3.txt","a"); 
//    int pos3 = fseek(fp2,0,SEEK_END);  
//    fprintf(fp2,"\n");
//    fprintf(fp2,"m=%d,n=%d,beta=%.2lf,tr=%.1lf,u=%.1lf,interval=100\n",m,n,beta,tr,u);
//    for(i=0;i<arrsize;i++)
//    {
//    	double tmp = (double)((int)((arronline[i]/arroffline[i]+0.005)*100))/100;
//    	double tmp1 = (double)((int)(((1+2*gama_online_timely[i])+0.005)*100))/100;
//    	fprintf(fp2,"%.2lf		%.2lf		%.2lf		%.2lf		%.2lf\n",arronline[i],arroffline[i],gama_online_timely[i],tmp,tmp1);
//	}
//    fclose(fp2); 
//    for(i=0;i<arrsize;i++)
//    printf("on/off=%.2lf  ",(double)((int)((arronline[i]/arroffline[i]+0.005)*100))/100);
    
//	printf("numoftransfer=%d\n",numoftransfer);
//	printf("numoftransfer_off=%d\n",numoftransfer_off);
//	printf("\n The off_line_cost is%.1lf",off_line_cost);
//	printf("\n The on_line_cost_default is%.1lf",on_line_cost);
//	printf("\n The on_line_cost_balance is%.1lf",on_line_cost_balance);
//	printf("\n The on_line_cost_priority is%.1lf",on_line_cost_priority);
//	printf("\n The on_line_cost_random is%.1lf",on_line_cost_random);
//	printf("\n on/off=%.3lf",on_line_cost/off_line_cost);
//	printf("\n The average cost of each request is%.3lf",off_line_cost/n);
//	printf("\n The ratio of transfer is %.2lf",ratiooftransfer);
//	printf("\n The ratio of off line transfer is %.2lf",ratiooftransfer_off);
//	printf("\n The beta is %.3lf",beta);
//	 
//	
	free(a);
	free(b);
	free(pserversize);
	free(replicas_on);
	free(replicas_off);
	free(pindex);
	free(serverorder);
	free(arroffline);
	free(arronline);
	free(gama_online_timely);
	return 0 ;
}
void Init()
{

//	char* mergefile = Merge_File("data3.txt","data4.txt","mergefile.txt");
	
//	printf("m=%d  \n",m);
	n= 0;
//	a = exponent(); //gaussrand()FormRequestSeq(); exponent()
//	b = GetServer();//FormServerSeq(); //intensive() uniform() GetServer()
//	beta = GetBeta();
//	serverorder = GetServerOrder();
//	n=totalelements;
//	arrsize = ceil((double)(n-1)/countinterval);
//	slidelen=30;
//	windowsize = 50;
//	printf("arrsize=%d\n",n);
//	arroffline = (double*)malloc(sizeof(double)*arrsize);
//	arronline = (double*)malloc(sizeof(double)*arrsize);
//	gama_online_timely = (double*)malloc(sizeof(double)*arrsize);
////	pindex = (PNODE*)malloc(sizeof(NODE)*n);
//	replicas_off = (int*)malloc(sizeof(int)*totalelements);
//	replicas_on = (int*)malloc(sizeof(int)*totalelements);
//	replicas_on_balance = (int*)malloc(sizeof(int)*totalelements);
//	replicas_on_priority = (int*)malloc(sizeof(int)*totalelements);
//	replicas_on_random = (int*)malloc(sizeof(int)*totalelements);
//	replicas_off[0] = 1;
//	replicas_on[0] = 1;
//	replicas_on_balance[0] = 1;
//	replicas_on_priority[0] = 1;
//	replicas_on_random[0] = 1;
	off_line_cost=0;
	on_line_cost = 0;
	on_line_cost_balance=0;
	on_line_cost_priority=0;
	on_line_cost_random=0;
	numoftransfer = 0;
	numoftransfer_balance=0;
	numoftransfer_priority=0;
	numoftransfer_off = 0;
	ratiooftransfer = 0;
	ratiooftransfer_off=0;
	pinvalid=(PNODE)malloc(sizeof(NODE));//创建一个无效请求节点
	if(NULL==pinvalid)//判断内存是否分配成功
		exit(-1);
	pinvalid->pnext =pinvalid;//pcover里面有指向无效节点的，输出时会判断该无效节点后面的那个节点是不是有效节点
	pinvalid->time = 0;
	pinvalid->server = -1;
//	PNODE pHead=(PNODE)malloc(sizeof(NODE));//创建第一个请求节点  0
//	if(NULL==pHead)
//		exit(-1);
//	pHead->time =0;
//	pHead->id=0;
//	pHead->pfront =pinvalid;//输入第一个节点时 要判断它前面是不是有有效节点
//	pHead->pnext=pinvalid;//初始化
////	pHead->serveway[2]={0}; 
//	for(i=0;i<m;++i)//数组的初始化，都指向那个无效节点
//	{
//		pLast[i]=pinvalid;
//		pHead->pcover[i]=pinvalid;//第0个点的输出
//	}
//	pLast[0]=pHead;
//	pHead->pcover[0]=pHead;
//	pindex[0]=pHead;//将第一个请求节点放入矩阵]
//	puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
}
int * FormServerSeq()
{
//	static int b1[n];
	int * b1 = (int*)malloc(sizeof(int)*n);
	b1[0]=0;
//	srand((int)time(0)); 
	for(i=1;i<n;++i)
	{
		b1[i]= (m*rand()/(RAND_MAX+1.0)) ;//    rand()%m;
	}
	for(i=0;i<n;++i)
//		printf("%d ",b1[i]);

	printf("\n");
	printf("\n");
	return b1;
}

double * FormRequestSeq()
{
//	static double a1[n];
	double * a1 = (double*)malloc(sizeof(double)*n);
	a1[0]=0;
//	srand((int)time(0)); 
	for(i=1;i<n;++i)
	{
		a1[i]=(double) ((int)((RAND*rand() /(RAND_MAX+1.0) + 0.05)*10)) /10;  //     rand()%RAND;
		for(j=0;j<i;++j)
		{
			if(a1[i]==a1[j])//剔除重复时间
			{
				i=i-1;
			}
		}
	}
	for(i=0;i<n-1;++i)//排序
	{
		for(j=i+1;j<n;++j)
		{
			if(a1[i]>a1[j])
			{
				num=a1[i];
				a1[i]=a1[j];
				a1[j]=num;
			}
		}
	}
	for(i=0;i<n;++i)
//		printf("%.1lf ",a1[i]);
	printf("\n");
	printf("\n");
	return a1;
}
int PivotIndex(int index)
{
	int cover[m]={0};
	int maxcoverindex=0;
	if(pindex[index]->pfront == pinvalid)
		return 0;	
	for(j=0;j<m;j++)
	{
		if(pindex[index]->pfront->pcover[j]->id == pindex[index]->pfront->id)
			continue;
		else if(pindex[index]->pfront->pcover[j]->pnext != pinvalid  && (pindex[index]->pfront->pcover[j]->pnext->id<pindex[index]->id) &&( pindex[index]->pfront->pcover[j]->pnext->serveway[0] == 1))
			cover[j] =  pindex[index]->pfront->pcover[j]->pnext->id; //满足要求的cover line  
	}
	maxcoverindex = cover[0];
	for(j=1;j<m;j++)
	{
		if(cover[j]>maxcoverindex)
			maxcoverindex = cover[j]; //这个循环变量不能跟compute_off_line一样  有交互 
	}
	return maxcoverindex;
}
double ComputeBi(int index)
{	
	double cachecost=0;
	double Bi=0;
	double bi=0;
	if(index==0)
		return 0;	
	for(k=1;k<=index;k++)
	{
		if(pindex[k]->pfront != pinvalid)//there exist a node before
		{
			cachecost = u*(pindex[k]->time - pindex[k]->pfront->time);
			if(cachecost < tr)
				bi = cachecost;
			else
				bi = tr;
		}
		else
			bi = tr;
		Bi+=bi;
	}
//	printf("bbbb""%d \n",Bi);
	return Bi;
}
double ComputeOff_line(int index)
{
//	printf("%d ",n);
//	printf("index=%d\n",index);
	int time=1;//记录整百 
	double transfercost=0;
	double cachecost2 = 0;
	bool flag;  //用来指示cache的类型  有K还是无K 
	double *D = (double*)malloc(sizeof(double)*n);
	for(i=0;i<n;i++) D[i]=0;
	double *C = (double*)malloc(sizeof(double)*n);
	for(i=0;i<n;i++) C[i]=0;
	double d;
	double D1=0;
	int K;
	int minindex;
	if(index ==0)
		return 0;
	for(i=1;i<=index;i++)
	{
		flag = false;
		minindex = -1;
		K = -1;
		double di[m+1] ={0};
		if(pindex[i]->pfront == pinvalid)
		{
			D[i]=1000000.0;//;DBL_MAX
		}
		else
		{
			di[0] = C[pindex[i]->pfront->id] + u*(pindex[i]->time - pindex[i]->pfront->time) + ComputeBi(i-1) - ComputeBi(pindex[i]->pfront->id);
			for(j=1;j<m+1;j++)
			{
				if((pindex[i]->pfront->pcover[j-1] != pinvalid) && (pindex[i]->pfront->pcover[j-1]->pnext != pinvalid) && (pindex[i]->pfront->pcover[j-1]->id < pindex[i]->pfront->id) && (pindex[i]->pfront->pcover[j-1]->pnext->id<pindex[i]->id)  && (pindex[i]->pfront->pcover[j-1]->pnext->id>pindex[i]->pfront->id))
				{
					if(pindex[i]->pfront->pcover[j-1]->pnext == pindex[i]) continue;
					di[j] = D[pindex[i]->pfront->pcover[j-1]->pnext->id] + u*(pindex[i]->time - pindex[i]->pfront->time) + ComputeBi(i-1) - ComputeBi(pindex[i]->pfront->pcover[j-1]->pnext->id);
					
				}
			}
//			puts("fffffffffffffffffffffffff");
//				D[i] = getMinimal(di);
			double min = di[0];
			for(j=1;j<m+1;j++)
			{
				if(di[j]<min && fabs(min-di[j])>0.000001 && di[j]!=0)
				{
					min = di[j];
					minindex = j;
				}
			
			}
			D[i] = min;
			if(minindex != -1)
				K = pindex[i]->pfront->pcover[minindex-1]->pnext->id;
			if(fabs(D[i]-di[0])<0.000001)
				flag=false;
			else flag = true; //存在k的情况 
		}
//		puts("ccccccccccccccccccccccc");
		//the request is servered by transfer
		transfercost = C[i-1]+u*(pindex[i]->time - pindex[i-1]->time)+tr;
		if(transfercost>D[i] || fabs(transfercost - D[i])<0.000001)
		{
			C[i]=D[i];
			pindex[i]->serveway[0] = 1;
			pindex[i]->serveway[1] = pindex[i]->pfront->id;
			if(flag)
			{
				for(j=K+1;j<pindex[i]->id;j++)
				{
					if(pindex[j]->pfront != pinvalid)//there exist a node before
					{
						cachecost2 = u*(pindex[j]->time - pindex[j]->pfront->time);
						if(cachecost2 < tr)
						{
							pindex[j]->serveway[0] = 1;
							pindex[j]->serveway[1] = pindex[j]->pfront->id;
						}							 
						else
						{
							pindex[j]->serveway[0] = 2;
							pindex[j]->serveway[1] = pindex[i]->pfront->id;
						}
							
					}
					else
					{
						pindex[j]->serveway[0] = 2;
						pindex[j]->serveway[1] = pindex[i]->pfront->id;
					}
				}
			}
			else
			{
				for(j=pindex[i]->pfront->id+1;j<pindex[i]->id;j++)
				{
					if(pindex[j]->pfront != pinvalid)//there exist a node before
					{
						cachecost2 = u*(pindex[j]->time - pindex[j]->pfront->time);
						if(cachecost2 < tr)
						{
							pindex[j]->serveway[0] = 1;
							pindex[j]->serveway[1] = pindex[j]->pfront->id;
						}		
						else
						{
							pindex[j]->serveway[0] = 2;
							pindex[j]->serveway[1] = pindex[i]->pfront->id;
						}
					}
					else
					{
						pindex[j]->serveway[0] = 2;
						pindex[j]->serveway[1] = pindex[i]->pfront->id;
					}
				}
			}		
		}		
		else
		{
			C[i]=transfercost;
			pindex[i]->serveway[0] = 2;	
			pindex[i]->serveway[1] = pindex[i]->id-1;
			numoftransfer_off++;
		}	
//		printf("C[i]=""%.3lf    ""D[i]=""%.3lf    ""transfercost=""%.3lf  loadcost=%.3lf  k=%d  \n",C[i],D[i],transfercost,0,K);			
						
	}
	//输出每个点的服务方式
//	bool flag3;
//	for(i=0;i<=index;i++)
//	{
//		int replicas = 0; 
//		flag3 = false;
////		if(i==0)
////			printf("\nThe request 0 is served by LOAD!\n");
////		if(pindex[i]->serveway[0] == 1)
////			printf("The request %d is served by CACHE from the request %d\n",i,pindex[i]->serveway[1]);
////		else if(pindex[i]->serveway[0] == 2)
////			printf("The request %d is served by TRANSFER from the request %d\n",i,pindex[i]->serveway[1]);
//		if(i==0)
//			continue;	
//		for(j=0;j<m;j++)
//		{
//			if((pindex[i]->pcover[j] != pinvalid) && (pindex[i]->pcover[j]->pnext->serveway[0] == 1) && (i != pindex[i]->pcover[j]->id))
//			{
//				replicas++;
////				printf("adaasdasdsfsdfsdfsdsaada");
//			}
//			
//		}
//		if(pindex[i]->serveway[0] == 1)
//			replicas++;
//		else if(pindex[i]->serveway[0] == 2)
//		{
//			for(j=0;j<m;j++)
//			{
//				if((pindex[i]->serveway[1] == pindex[i]->pcover[j]->id) && (pindex[i]->pcover[j]->pnext->serveway[0] == 1))
//					flag3=true;
//			}
//			if(!flag3)
//				replicas++;	
//		}	
////		printf("i=%d,  replicas=%d\n",i,replicas);
//		replicas_off[i] = replicas;
//	}
	double cost1 = C[index]; 
//	printf("cost=%.2lf\n",cost1);
//	free(C);
//	free(D);
	return cost1;
}

//double ComputeOff_line(int index)
//{
//	printf("%d ",n);
//	int time=1;//记录整百 
//	double transfercost=0;
//	double cachecost2 = 0;
//	bool flag;  //用来指示cache的类型  有K还是无K 
//	double *D = (double*)malloc(sizeof(double)*n);
//	for(i=0;i<n;i++) D[i]=0;
//	double *C = (double*)malloc(sizeof(double)*n);
//	for(i=0;i<n;i++) C[i]=0;
//	double d;
//	double D1=0;
//	int K;
//	if(index ==0)
//		return 0;
//	for(i=1;i<=index;i++)
//	{
//		flag = false;
//		if(pindex[i]->pfront == pinvalid)
//		{
//			D[i]=1000000.0;//;DBL_MAX
//		}
//		else
//		{
//			//compute Di
//			//if no cache spanning across rpi					
//			K = PivotIndex(i);
//			if(K == 0)
//			{			
//				D[i] = C[pindex[i]->pfront->id] + u*(pindex[i]->time - pindex[i]->pfront->time) + ComputeBi(i-1)- ComputeBi(pindex[i]->pfront->id);								
////				printf("no cross cost=""%.3lf    ",D[i]);	 
//			} 				
//			else
//			{
//				flag = true;				
//				D[i] = D[K] + u*(pindex[i]->time - pindex[i]->pfront->time) + ComputeBi(i-1) - ComputeBi(K);
////				printf("d= ""%.3lf      ""K= ""%d  ",D[i],K);			
//			}						
//		}
//		//the request is servered by transfer
//		transfercost = C[i-1]+u*(pindex[i]->time - pindex[i-1]->time)+tr;
//		if(transfercost>D[i] || fabs(transfercost - D[i])<0.000001)
//		{
//			C[i]=D[i];
//			pindex[i]->serveway[0] = 1;
//			pindex[i]->serveway[1] = pindex[i]->pfront->id;
//			if(flag)
//			{
//				for(j=K+1;j<pindex[i]->id;j++)
//				{
//					if(pindex[j]->pfront != pinvalid)//there exist a node before
//					{
//						cachecost2 = u*(pindex[j]->time - pindex[j]->pfront->time);
//						if(cachecost2 < tr)
//						{
//							pindex[j]->serveway[0] = 1;
//							pindex[j]->serveway[1] = pindex[j]->pfront->id;
//						}							 
//						else
//						{
//							pindex[j]->serveway[0] = 2;
//							pindex[j]->serveway[1] = pindex[i]->pfront->id;
//						}
//							
//					}
//					else
//					{
//						pindex[j]->serveway[0] = 2;
//						pindex[j]->serveway[1] = pindex[i]->pfront->id;
//					}
//				}
//			}
//			else
//			{
//				for(j=pindex[i]->pfront->id+1;j<pindex[i]->id;j++)
//				{
//					if(pindex[j]->pfront != pinvalid)//there exist a node before
//					{
//						cachecost2 = u*(pindex[j]->time - pindex[j]->pfront->time);
//						if(cachecost2 < tr)
//						{
//							pindex[j]->serveway[0] = 1;
//							pindex[j]->serveway[1] = pindex[j]->pfront->id;
//						}		
//						else
//						{
//							pindex[j]->serveway[0] = 2;
//							pindex[j]->serveway[1] = pindex[i]->pfront->id;
//						}
//					}
//					else
//					{
//						pindex[j]->serveway[0] = 2;
//						pindex[j]->serveway[1] = pindex[i]->pfront->id;
//					}
//				}
//			}		
//		}		
//		else
//		{
//			C[i]=transfercost;
//			pindex[i]->serveway[0] = 2;	
//			pindex[i]->serveway[1] = pindex[i]->id-1;
//			numoftransfer_off++;
//		}							
////		printf("i=%d %.4lf   C[i]=""%.3lf    ""D[i]=""%.3lf    ""transfercost=""%.3lf  \n",i,pindex[i]->time,C[i],D[i],transfercost);
////		if(i==index)
////		{
////			arroffline[arrsize-1] = C[index];
//////			printf("arroffline=%.2lf  ",	C[i]);
////			break;
////		}
////		if(i==(time*countinterval))
////		{
////			arroffline[time-1] = C[i];
//////			printf("arroffline=%.2lf  ",	C[i]);
////			time++;
////		}			
//	}
//	//输出每个点的服务方式
////	bool flag3;
////	for(i=0;i<=index;i++)
////	{
////		int replicas = 0; 
////		flag3 = false;
//////		if(i==0)
//////			printf("\nThe request 0 is served by LOAD!\n");
//////		if(pindex[i]->serveway[0] == 1)
//////			printf("The request %d is served by CACHE from the request %d\n",i,pindex[i]->serveway[1]);
//////		else if(pindex[i]->serveway[0] == 2)
//////			printf("The request %d is served by TRANSFER from the request %d\n",i,pindex[i]->serveway[1]);
////		if(i==0)
////			continue;	
////		for(j=0;j<m;j++)
////		{
////			if((pindex[i]->pcover[j] != pinvalid) && (pindex[i]->pcover[j]->pnext->serveway[0] == 1) && (i != pindex[i]->pcover[j]->id))
////			{
////				replicas++;
//////				printf("adaasdasdsfsdfsdfsdsaada");
////			}
////			
////		}
////		if(pindex[i]->serveway[0] == 1)
////			replicas++;
////		else if(pindex[i]->serveway[0] == 2)
////		{
////			for(j=0;j<m;j++)
////			{
////				if((pindex[i]->serveway[1] == pindex[i]->pcover[j]->id) && (pindex[i]->pcover[j]->pnext->serveway[0] == 1))
////					flag3=true;
////			}
////			if(!flag3)
////				replicas++;	
////		}	
//////		printf("i=%d,  replicas=%d\n",i,replicas);
////		replicas_off[i] = replicas;
////	}
//	double cost1 = C[index]; 
////	free(C);
////	free(D);
//	return cost1;
//}

int * GetServerSize_except()
{
	int *p = (int*)malloc(sizeof(int)*arrsize);
	for(i=0;i<arrsize;i++) p[i]=0;
	int count = 0;
	int l;
	int k=1;
	extern int sizeofserver[m];
	for(l=0;l<arrsize;l++)
	{
		while(pindex[k]->id <= (l+1)*countinterval)
		{
			if(pindex[k]->pfront != pinvalid)
			{
				if( !(fabs(u*(pindex[k]->time-pindex[k]->pfront->time))-tr > 0))
				{
//					printf("%.1lf,%.1lf",fabs(u*(pindex[k]->time-pindex[k]->pfront->time)),tr);
//					printf("k=%d count=%d \n",k,count);
					count++;
				}
			}
			if(k==(totalelements-1))
			{
				p[arrsize-1] = totalelements-1- count;
				break;
			}
			k++;	
		}
		if(k!=(totalelements-1))
			p[l] = (l+1)*countinterval - count;
		
	} 
	for(i=0;i<arrsize;i++)
//	printf("aa%d  ",p[i]);
	return p;
} 
void GetAveReplicas(int* replicas)
{
//	for(i=0;i<n;i++)
//	printf("%d  ",replicas[i]);
//	FILE *fp;  
//    fp=fopen("avereplicas.txt","a"); 
//    int pos = fseek(fp,0,SEEK_END);  
//    fprintf(fp,"\n");
//    
//	fprintf(fp,"%d	%d	%.2lf	%.0lf	%.1lf	%.1lf  %d   %d
//				m,	n,	beta,threshold,  tr,	 u,	);
//	fclose(fp);
	int tmp;
	double avereplicas;
	bool flag = false;
	printf("adasfsdwerwerfsdsda");
//		tmp = replicas[i*slidelen];
	for(i=0;i<n;i++)
	{
		tmp = 0;
		for(j=0;j<windowsize;j++)
		{
			if(i*slidelen+j == n)
			{
				flag = true; 
				tmp += replicas[i*slidelen+j];
				avereplicas = (double)tmp/(j+1);
//				printf("tmp=%d,j=%d  ",tmp,j);
//				printf("%.1lf  ",avereplicas);
				break;
			}
			else
				tmp += replicas[i*slidelen+j];
//			printf("%d  ",tmp);
		}
		if(flag) break;
		else
		{
			avereplicas = (double)tmp/(j+1);
//			printf("%.1lf  ",avereplicas);
		} 	
	}
}

void Get_doublelinked_list(char *path)
{
//	puts(path);
//	printf("%d ",p);
	FILE * fp =  fopen(path,"r");
	double *a;
	int *c;
	double ti;
	int si;
	int *b;
	int circu1 = 0;
	int i =0;
	char buf[32];
	//文件行数
	while(fgets(buf, 32, fp)) 
		n++;
	a = (double*)malloc(sizeof(double)*n);
	b = (int *)malloc(sizeof(int)*n);
	c = (int *)malloc(sizeof(int)*n);
//	printf("linecnt=%d\n",n); 
	pindex = (PNODE*)malloc(sizeof(NODE)*n);

	fseek(fp,0,SEEK_SET);
	char * s1 = fgets(buf,32,fp);
	while(s1 != NULL)
	{
		char delims[] = ",";
		char *result = NULL; 
		result = strtok(buf,delims);
		while( result != NULL)
		{
			if(circu1==0)
			{
				sscanf(result,"%d",&b[i]); 
			}
			else if(circu1 == 1)
				sscanf(result,"%lf",&a[i]);
//			printf("ssss%s %d ",result,circu1);
        	result = strtok( NULL, delims );
			circu1++; 
		}
		if(circu1==3) c[i] = 1;
		if(circu1==4) c[i] = 2;
		i++; 
		circu1 = 0;
		s1 = fgets(buf,32,fp);
	}
//	for(i=0;i<n;i++)
//	{
//		printf("%d,%lf\n",b[i],a[i]);
//	}
	
	//创建双向链表
	PNODE pHead=(PNODE)malloc(sizeof(NODE));//创建第一个请求节点  0
	if(NULL==pHead)
		exit(-1);
	pHead->time =0;
	pHead->id=0;
	pHead->server = 0;
	pHead->pfront =pinvalid;//输入第一个节点时 要判断它前面是不是有有效节点
	pHead->pnext=pinvalid;//初始化
	pHead->nodesize = 2;
//	pHead->serveway[2]={0}; 
	for(i=0;i<m;++i)//数组的初始化，都指向那个无效节点
	{
		pLast[i]=pinvalid;
		pHead->pcover[i]=pinvalid;//第0个点的输出
	}
	pLast[0]=pHead;
	pHead->pcover[0]=pHead;
	pindex[0]=pHead;//将第一个请求节点放入矩阵]
	
	for(i=1;i<n;i++)//第一个点已经建好
	{
//		printf("a%d ",i); 
		PNODE pNew=(PNODE)malloc(sizeof(NODE));
		if(NULL==pNew)
			exit(-1);
		si=b[i-1];
		ti=a[i-1];
//		printf("%d,%.1lf ",si,ti);
		pindex[i]=pNew;//将该请求节点放入数组中
		pNew->pnext =pinvalid;
		pNew->pfront=pinvalid;
		pNew->id = i;
		pNew->server = si;
		pNew->serveway[0] = 2;//默认服务方式是transfer 
		pNew->time = ti;
		pNew->nodesize = c[i-1];
		if(pLast[si]!=pinvalid )//说明新节点前面有节点
		{
			pLast[si]->pnext =pNew;//前后请求建立联系
			pNew->pfront =pLast[si];
		}
			
		pLast[si]=pNew;//更新
		memcpy(pindex[i]->pcover,pLast,m*sizeof(int*));//把pLast中的指针赋给pcover
//		printf("%.4lf ",pNew->time);
	}

//以一行代表一个服务器输出请求节点 反了 因为pLast里面是每个服务器最后一个请求
//	puts("ssssssssssssssssssssssssssssssssssss"); 
//	for(j=0;j<m;++j)
//	{
//		PNODE p=(PNODE)malloc(sizeof(NODE));
//		p=pLast[j];
//		if(p == pinvalid)
//			continue;
//		else
//		{
//			printf("%.4lf ",p->time );
//			for(k=0;k<n;++k)
//			{
//				if(p->pfront !=pinvalid)
//				{
//					printf("%.4lf ",p->pfront->time);
//					p=p->pfront ;
//				}
//			}
//		}
//		printf("\n");
//	}
	printf("\n");
	fclose(fp);
	free(c);
	free(a);
	free(b);
}

void compute_cost_correlation(double *cost_cor,double *cost)  //不拆包，无论是单独出现还是一起出现，都用数据包服务 
{
	char * inputfile = "G:/trafficData20160510/multi_data/数据2/package110_s4.txt"; //package110_s3.txt
	double cost_base[DATASIZE];
	for(i=0;i<DATASIZE;i++) cost_base[i] = cost[i];
	FILE * fp3 = fopen(inputfile,"r");
	int* arr;
	char buf3[32];
	if(fp3 == NULL )
 	{
 		printf("open file failed!\n");
 		exit(1);
	} 
	char * s = NULL;//fgets(buf,32,fp);
	while((s=fgets(buf3,32,fp3)) != NULL)
	{
//		puts(s);
		static int q = 0;
		int datacount = (strlen(s)-1)/2;
//		printf("datacount=%d\n",datacount);
		if(datacount==1 || datacount==0) continue;
		else
		{
			int t=0;
			arr = (int*)malloc(sizeof(int)*datacount);
			char path2[7] = "";
			char delims3[] = " ";
			char *result3 = NULL; 
			result3 = strtok(buf3,delims3);
			while(result3!=NULL)
			{
				arr[t++]= atoi(result3)-1;
				result3 = strtok(NULL,delims3);
			}
//			while((result3 != NULL) && (result3 != "\0"))//
			for(i=0;i<datacount;i++)
			{
				char path1[7] = "";
				char outputpath[7] = "";
//				char tmp3[7] = "";
				
				itoa(q+20,outputpath,10);
				strcat(outputpath,".txt");//outputfile  20.txt
//				puts(outputpath);

				int tmppath = arr[i];
				itoa(tmppath,path1,10);   //7.txt 8.txt
				strcat(path1,".txt");
				
//				printf("path1=%s,path2=%s,outputpath=%s\n",path1,path2,outputpath);
				Merge_File(path1,path2,outputpath);

				strcpy(path2,outputpath);
				q++;
			}
			double sum = 0;
			Init(); 
			Get_doublelinked_list(path2);
			for(i=0;i<datacount;i++)
			{
				sum+=cost[arr[i]];
				cost_cor[arr[i]]=0; //代价清0 
				cost_base[arr[i]]=0;
			}
			cost_cor[arr[0]] = (ComputeOff_line(n-1)*datacount*THETA)/n;
			cost_base[arr[0]] = sum;
			ratiooftransfer_off = (double)numoftransfer_off / n *100;
//			printf("n=%d,ratiooftransfer_off=%lf",n,ratiooftransfer_off);
			free(arr);
//			system("pause");			
//			s = fgets(buf,32,fp);
		}
	}
//	printf("\nbase:");
//	for(i=0;i<DATASIZE;i++) printf("%lf  ",cost_base[i]);
//	printf("\npack:");
}


void compute_cost_DPgreedy(double *cost_cor,double *cost)  //DP_Greedy 
{
	char * inputfile = "package110_s4.txt"; // package110_s3.txt
	double cost_base[DATASIZE];
	for(i=0;i<DATASIZE;i++) cost_base[i] = cost[i];
	FILE * fp3 = fopen(inputfile,"r");
	int* arr;
	int* arr_reverse;
	char buf3[32];
//	printf("lineCnt=%d\n",lineCnt);
	if(fp3 == NULL )
 	{
 		printf("open file failed!\n");
 		exit(1);
	} 
	char * s = NULL;//fgets(buf,32,fp);
	while((s=fgets(buf3,32,fp3)) != NULL)
	{
//		puts(s);
		static int q = 0;
		int datacount = (strlen(s)-1)/2;
//		printf("datacount=%d\n",datacount);
		if(datacount==1 || datacount==0) continue;
		else
		{
			int t=0;
			arr = (int*)malloc(sizeof(int)*datacount);
			arr_reverse= (int*)malloc(sizeof(int)*datacount);
			char path2[7] = "";
			char delims3[] = " ";
			char *result3 = NULL; 
			result3 = strtok(buf3,delims3);
			while(result3!=NULL)
			{
				arr[t++]= atoi(result3)-1;
				result3 = strtok(NULL,delims3);
			}
//			while((result3 != NULL) && (result3 != "\0"))//
			for(i=0;i<datacount;i++)
			{
				char path1[7] = "";
				char outputpath[7] = "";
//				char tmp3[7] = "";
				
				itoa(q+50,outputpath,10);
				strcat(outputpath,".txt");//outputfile  20.txt
//				puts(outputpath);

				int tmppath = arr[i];
				itoa(tmppath,path1,10);   //7.txt 8.txt
				strcat(path1,".txt");
				
//				printf("path1=%s,path2=%s,outputpath=%s\n",path1,path2,outputpath);
				
//				printf("arr: arr[0]=%d	arr[1]=%d\n",arr[0],arr[1]);
				for(j=0;j<datacount;j++) arr_reverse[j] = arr[datacount-j-1];
				Merge_File2(path1,path2,outputpath,arr_reverse);
//				system("pause");
				strcpy(path2,outputpath);
				q++;
			}
			//得到两个请求同时出现序列的文件 
			char outputpath2[7]=""; //存放共同出现的请求 
			char outputpath3[7]=""; //存放1
			char outputpath4[7]="";  //存放2  
			itoa(q+50,outputpath2,10);
			strcat(outputpath2,".txt");//outputfile  20.txt
//			puts(outputpath2);
			
			q++;
			itoa(q+50,outputpath3,10);
			strcat(outputpath3,".txt");//outputfile  20.txt
			
			q++;
			itoa(q+50,outputpath4,10);
			strcat(outputpath4,".txt");//outputfile  20.txt
			q++;
				
			getCoExistence(path2,outputpath3,outputpath4,outputpath2,arr);  //按照请求类型（同时请求两个数据项还是其中一个）存为不同文件 
			Init();
			Get_doublelinked_list(outputpath2);
			double cost_co = ComputeOff_line(n-1)*datacount*THETA;
//			printf("cost_co=%.1lf\n",cost_co);
			int n3 =n;
		
			Init();
			Get_doublelinked_list(outputpath3);
			double cost_single1 = computeGreedy(n-1);
//			printf("cost_single1=%lf\n",cost_single1);
			int n1 = n;
			
			Init();
			Get_doublelinked_list(outputpath4);
			double cost_single2 = computeGreedy(n-1);
//			printf("cost_single2=%lf\n",cost_single2);
			int n2 =n;
			
			double sum = 0;
			for(i=0;i<datacount;i++)
			{
				sum+=cost[arr[i]];
				cost_cor[arr[i]]=0; //代价清0 
				cost_base[arr[i]]=0;
			}
			
			
			cost_cor[arr[0]] = (cost_co+cost_single1+cost_single2)/(n1+n2-n3);
			cost_base[arr[0]] = sum/(n1+n2-n3);
//			cost_cor[arr[0]] = (cost_co+cost_single1+cost_single2);
//			cost_base[arr[0]] = sum;
			
			
			ratiooftransfer_off = (double)numoftransfer_off / n *100;
//			printf("n=%d,ratiooftransfer_off=%lf",n,ratiooftransfer_off);
			free(arr);
			free(arr_reverse);
//			system("pause");			
//			s = fgets(buf,32,fp);
		}
	}
	printf("\nbase:");
	for(i=0;i<DATASIZE;i++) printf("%lf  ",cost_base[i]);
	printf("\ndpgreedy:");
}

double computeGreedy(int index)
{
	double cost = 0;
	double eventtime[m]={0}; 
	double cachecost;
	double transfercost;
	double base = tr*2*THETA;
	double mincost;
	for(i=1;i<=index;i++)
	{
		mincost = 0;
		cachecost=0;
		transfercost=0;
//		printf("%d,%lf\n",pindex[i]->id,pindex[i]->time);
		if(pindex[i]->nodesize==2) 
		{
			eventtime[pindex[i]->server] = pindex[i]->time;
			continue;
		}
		if(pindex[i]->pfront==pinvalid) cachecost = 100000.0;
		else cachecost = u*(pindex[i]->time-eventtime[pindex[i]->server]);//u*(pindex[i]->time-pindex[i]->pfront->time);
		transfercost = u*(pindex[i]->time-pindex[i-1]->time)+tr;
		if(cachecost<base && fabs(cachecost-base)>0.000001)
			mincost=cachecost;
		else
		{
			mincost = base;
		}
		if(mincost>transfercost && fabs(mincost-transfercost)>0.000001)
		{
			mincost=transfercost;
			eventtime[pindex[i-1]->server] = pindex[i]->time;
		}
		cost+=mincost;
		eventtime[pindex[i]->server] = pindex[i]->time;
//		printf("cachecost=%lf,transfercost=%lf,mincost=%lf,cost=%lf\n",cachecost,transfercost,mincost,cost)	;
	}
	return cost;
}


