#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "SAT.h"
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <malloc.h>
#include <fcntl.h>
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


#define TRUE 1
#define FALSE 0
#define MAXSIZE 999    /* initial allocate memory space */
#define MAXEDGE 999
#define MAXVEX 999


typedef int Status;	/* Status is a function type */
typedef int Boolean; /* Boolean type, the value includes TRUE or FALSE */

typedef int VertexType; /* vertex type "int" */
typedef int EdgeType; /* edge type */

/* Adjvex List Type */
typedef struct
{
	VertexType vexs[MAXVEX]; /* Vertex List */
	EdgeType arc[MAXVEX][MAXVEX];/* an array to help creat the linked list */
	int numVertexes, numEdges; 
}MGraph;

/* Edge Lists structure****************** */
typedef struct EdgeNode /*define edge node structure */
{
	int adjvex;    /* the field of the adjvex */
	struct EdgeNode *next; /* a pointer pointing to the next edge node */
}EdgeNode;

typedef struct VertexNode //define a vertex Node
{
	int in;	
	int data; 
	int record[MAXVEX];  
	EdgeNode *firstedge;   //this edge pointer pointing to the adjvex
}VertexNode, AdjList[MAXVEX];

typedef struct
{
	AdjList adjList;
	int numVertexes,numEdges; 
}graphAdjList,*GraphAdjList;


typedef struct
{
	int data[MAXSIZE];
	int front;    	//define a queue structure
	int rear;	
}Queue;


/* initialize the queue*/
Status InitQueue(Queue *Q)
{
	Q->front=0;
	Q->rear=0;
	return  1;
}

/* define the ligic of an empty queue */
Status QueueEmpty(Queue Q)
{
	if(Q.front==Q.rear) 
		return TRUE;
	else
		return FALSE;
}

/* this is how to add Queue */
Status EnQueue(Queue *Q,int e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	
		return 0;
	Q->data[Q->rear]=e;			
	Q->rear=(Q->rear+1)%MAXSIZE;
	return  1;
}

/* define how to delete an element from Queue */
Status DeQueue(Queue *Q)
{
	int temp;
	if (Q->front == Q->rear)	
		return 0;
	temp=Q->data[Q->front];		
	Q->front=(Q->front+1)%MAXSIZE;	
	return  temp;
}
/* ****************************************************** */

//compute the vertexNumber

int VStr(char str[10000])
{
    int str_size1;
    int str_size;
    int V_flag;

    for (str_size = 0;str[str_size]!='\0';str_size++)
            {
                str_size1 = str_size;
            }


    int VertexNumber = 0;

    for (V_flag = 2; V_flag < str_size1;V_flag++)
    {
        VertexNumber = (int)(VertexNumber + (str[V_flag]-'0') * pow(10,str_size1-V_flag-1));
    }


    return VertexNumber;
}

int ENum(char str[10000])
{
    int str_size;
    int edgeNum = 0;


    for (str_size = 0;str[str_size]!='\0';str_size++)
            {
                if(str[str_size] == '<')
                    edgeNum++;
            }

    return edgeNum;
}


int CreateMGraph(MGraph *G, int Edge_num, int VertexNum, char str[10000]) //str[100] is E ****
{
	int i, j,k;
	G->numEdges=Edge_num;
	G->numVertexes=VertexNum;

//	printf("%d %d\n",Edge_num,VertexNum);

	
	for(i = 0;i<VertexNum;i++)
    {
        G->vexs[i]= i;
//        printf("%d\n",G->vexs[i]);
    }


	for (i = 0; i < G->numVertexes; i++)
	{
		for ( j = 0; j < G->numVertexes; j++)
		{
			G->arc[i][j]=0;
		}
	}

	//obtain the edge info.
    int str_s1;
    int str_s;
    int E_flag;

    for (str_s = 0;str[str_s]!='\0';str_s++)
            {
                str_s1 = str_s + 1;
            }

    //establish the edges

    E_flag = 2;


    for (k = 0; k < Edge_num; k++)
    {
        int flag1 = 1;
        int flag2 = 1;
        while(str[E_flag]!= '<')
        {
            E_flag++;
        }

        while(str[E_flag+flag1]!=',')
        {
            flag1++;
        }

        while(str[E_flag+flag1+flag2]!= '>')
        {
            flag2++;
        }

        int E1,E2;
        int edge_left = 0;
        int edge_right = 0;
        for (E1 = E_flag + 1; E1 < E_flag + flag1;E1++)
        {
            edge_left = (int)(edge_left + (str[E1]-'0') * pow(10,E_flag + flag1-1-E1));
        }

        for (E2 = E_flag + flag1 + 1; E2 < E_flag + flag1 + flag2;E2++)
        {
            edge_right = (int)(edge_right + (str[E2]-'0') * pow(10,E_flag + flag1 + flag2-1-E2));
        }

        int error_flag = 0;
        if(edge_left>=VertexNum || edge_right>=VertexNum)
            {
                printf("Error: one or more vertexes do not exist\n");
		fflush(stdout);
		return 0;
                error_flag = 1;
                break;
            }
        if(error_flag ==1)
        {
            continue;
        }
        G->arc[edge_left][edge_right]=1;
 
        G->arc[edge_right][edge_left]=1;
        E_flag = E_flag+flag1+flag2;
//        printf("%d %d\n",edge_left,edge_right);
    }

	return 1;

}


void CreateALGraph(MGraph G,GraphAdjList *GL)
{
	int i,j;
	EdgeNode *e;

	*GL = (GraphAdjList)malloc(sizeof(graphAdjList));

	(*GL)->numVertexes=G.numVertexes;
	(*GL)->numEdges=G.numEdges;
	for(i= 0;i <G.numVertexes;i++) 
	{
		(*GL)->adjList[i].in=0;
		(*GL)->adjList[i].data=G.vexs[i];
		(*GL)->adjList[i].firstedge=NULL; 
		for(j=0;j<MAXVEX;j++)
        {
            (*GL)->adjList[i].record[j] = -1;
        }

	}

	for(i=0;i<G.numVertexes;i++) 
	{
		for(j=0;j<G.numVertexes;j++)
		{
			if (G.arc[i][j]==1)
			{
				e=(EdgeNode *)malloc(sizeof(EdgeNode));
				e->adjvex=j;
				e->next=(*GL)->adjList[i].firstedge;
				(*GL)->adjList[i].firstedge=e;	
				(*GL)->adjList[j].in++;
			}
		}
	}

}

Boolean visited[MAXSIZE]; 


void BFSTraverse(GraphAdjList GL,int firstNode,int lastNode)
{
	int i,current;
//	printf("%d %d\n",firstNode,lastNode);
    EdgeNode *p;
    int error_flag1 = 0;

	Queue Q;
	for(i = 0; i < GL->numVertexes; i++)
       	visited[i] = FALSE;
    InitQueue(&Q);

    visited[firstNode]=TRUE;   //to directly traverse from the first Node as speicified
//    printf("%d",GL->adjList[firstNode].data);

    EnQueue(&Q,firstNode);

    while(!QueueEmpty(Q))
        {
            current = DeQueue(&Q);
//            printf("%d-",current);
            p = GL->adjList[current].firstedge;	


            while(p)
            {
                   if(!visited[p->adjvex])	
                {
                    visited[p->adjvex]=TRUE;
//                    printf("-%d",GL->adjList[p->adjvex].data);
                    EnQueue(&Q,p->adjvex);

                    for(i=0;i<MAXVEX;i++)
                    {
                        if(GL->adjList[current].record[i] != -1)
                            GL->adjList[p->adjvex].record[i] = GL->adjList[current].record[i];
                        else
                        {
                            GL->adjList[p->adjvex].record[i] = current;
                            break;
                        }
                    }

                    if(GL->adjList[p->adjvex].data == lastNode)
                    {
                        printf("%d",GL->adjList[p->adjvex].record[0]);//this is to directly record the path of the BFS traverse from the firstNode to the lastNode
                        for(i=1;GL->adjList[p->adjvex].record[i]!=-1;i++)
                        {
                            printf("-%d",GL->adjList[p->adjvex].record[i]);
                        }
                        printf("-%d\n",lastNode);

                        error_flag1 = 1;

                        break;
                    }
                }
                p = p->next;
            }
        }
        if(error_flag1 == 0)
            {
                printf("Error: There is no path between the two nodes as given!\n");//if after BFS the last node is not found, then there is an error
            }
}


/*build up the vertex cover solution_CNFSAT*/
void* VerCover_CNFSAT(void* arg) 
{
	MGraph* G = (MGraph*) arg; 
	
	int VerticeNum,EdgeNum,i,j,m;
	int vNum;
	VerticeNum = G -> numVertexes;  
	EdgeNum = G -> numEdges;
	//printf("%d %d\n",VerticeNum,EdgeNum);	

	int pip[2],bak;
	pipe(pip);
	bak = dup(STDOUT_FILENO);
	dup2(pip[1],STDOUT_FILENO); 

	int k,o,p,q,temp,temp1;	
	int d[2];


	
	for(k=1;k<=VerticeNum;k++)
	{

		int *c = (int *)malloc(VerticeNum * sizeof(int));
		SAT_Manager mgr = SAT_InitManager();
//		printf("%d %d\n", k, VerticeNum * k);

		SAT_SetNumVariables(mgr,VerticeNum * k); //remember that the index starts from 1 not 0, so we need to decrease by 1 when print out to adapt to the habits of assginement 2.
		
		for (i = 1;i<=k;i++){ //to find out the first clause type
			for (j = 1;j<=VerticeNum;j++){
				temp = VerticeNum * (i-1) + j;  //x-ji
				c[j-1] = (temp << 1);
				//printf("%d ",temp);				
			} //for (j)
			//printf("\n");
			SAT_AddClause(mgr,c,VerticeNum);
		}//for (i) ; k clauses have been set up
		
		free(c);
		c = NULL;

		/*second clause type*/
		for (m = 1; m <= VerticeNum; m++){
			for(p = 1; p < k; p++){
				for(q = p + 1;q<=k;q++){
					temp = VerticeNum * (p - 1) + m;//x-mp
					d[0] = (temp << 1)+1;  //!x-mp
					//printf("%d %d ",temp,d[0]);
					temp = VerticeNum * (q - 1) + m; //x-mq
					d[1] = (temp << 1)+1;  //!x-mq
					//printf("%d %d\n",temp,d[1]);
					SAT_AddClause(mgr,d,2);	
				}	
			}
		}	
	
		/*third clause type*/
		for (m = 1; m <= k; m++){
			for(p = 1; p < VerticeNum; p++){
				for(q = p + 1;q<=VerticeNum;q++){
					temp = VerticeNum * (m - 1) + p;//x-pm
					d[0] = (temp << 1)+1;  //!x-pm
					//printf("%d %d ",temp,d[0]);
					temp = VerticeNum * (m -1) + q; //x-qm
					d[1] = (temp << 1)+1;  //!x-qm
					//printf("%d %d\n",temp,d[1]);
					SAT_AddClause(mgr,d,2);	
				}	
			}
		}	

		int *e = (int *)malloc(2*k*sizeof(int));
	
		/*fourth clause type*/
		for(i=0;i<G->numVertexes;i++) 
		{
			for(j=i+1;j<G->numVertexes;j++)
			{	
				if (G->arc[i][j]==1)
				{
					for(o = 1;o <= k; o++)
					{
						temp = VerticeNum * (o-1) + i + 1;  //x-oi; i from graph is from 0, SAT is from 1
						e[o-1] = (temp << 1);//xi(1...k)
						temp1 = VerticeNum * (o-1) + j + 1;
						e[o-1+k] = (temp1 << 1);//xj(1...k)	
						//printf("%d %d %d %d %d %d\n",i,j,temp,temp1,e[o-1],e[o-1+k]);						
					}
					SAT_AddClause(mgr,e,2*k);
				}
			}	
		}

		free(e);
		e = NULL;	
		
		
		int result = SAT_Solve(mgr);
		
		if(result == SATISFIABLE)
		{
			dup2(bak, STDOUT_FILENO);
			int n = SAT_NumVariables(mgr);
			//printf("%d\n",n);
			int *v = (int *)malloc(n*sizeof(int));
			
			for(i = 1; i <= n;i++)
			{
				int a = SAT_GetVarAsgnment(mgr,i);
				if(a==1){
					v[i-1] = i;
					//printf("%d %d ",i,v[i-1]); 
				}				
				else if(a==0){
					v[i-1] = -1*i; 
					//printf("%d %d ",i,v[i-1]);
				}
				else{
					printf("Error!"); 
					fflush(stdout);
				}
			}
			//printf("\n");
			printf("CNF-SAT-VC: ");
			vNum = 0;
			for (m = 1; m <= VerticeNum; m++){
				for(p = 1; p <= k; p++){
					i = (p-1) * VerticeNum + m;
					if(v[i-1] > 0)
					{
						printf("%d ",m-1);
						vNum = vNum + 1; //to calculate the total number of vertexes
					}
				}
			}
			printf("\n");
			fflush(stdout);
			free(v);
			v = NULL;
			break; 
		}		
	}
	//h -> vNum = vNum;
	return NULL;
}

int calMax(int VerticeNum, int* a){
	int maxNode,maxValue,i;
	maxValue = a[0];
	maxNode = 0;

	for(i=1;i<VerticeNum;i++)
	{
		if(a[i]>maxValue)
		{
			maxValue = a[i];
			maxNode = i;
		}
	}
	return maxNode;
}

void* VerCover_APPROX1(void* arg) 
{
	MGraph* G = (MGraph*) arg; 
	
	int VerticeNum,EdgeNum,i,j,m,maxNode;
	int vNum;
	//MGraph G;	
	VerticeNum = G -> numVertexes;  
	EdgeNum = G -> numEdges;
	
	/*a_1d is to store the time that one node appear in an edge*/
	int *a_1d = (int*)malloc(sizeof(int)*VerticeNum);
	for(i=0;i<VerticeNum;i++)
	{
		a_1d[i] = 0;
	}
	/*a_2d is to store whether a node is selected as a vertex*/
	int *a_2d = (int*)malloc(sizeof(int)*VerticeNum);
	for(i=0;i<VerticeNum;i++)
	{
		a_2d[i] = 0;
	}


	m = 1;
	while(m!=0){
		/*calulating the vertex of the highest degree*/
		m = 0;
		
		for(i=0;i< VerticeNum-1;i++) 
		{
			if (a_2d[i] == 1)
			{
				continue;
			}
			else
			{	
				for(j=i+1;j< VerticeNum;j++)
				{
					if (a_2d[j] == 1)
					{
						continue;
					}
					else
					{		
						if (G->arc[i][j]==1)
						{
							a_1d[i] = a_1d[i]+1;					
							a_1d[j] = a_1d[j]+1;
							m++;			
						}
					}
				}
			}
		}
		maxNode=calMax(VerticeNum, a_1d); //calculate which node is of the highest degree
		a_2d[maxNode] = 1;

	}

	/*to print out the vertexes*/
	printf("APPROX-VC-1: ");
	vNum = 0;
	for (i = 0; i < VerticeNum; i++)
	{
		if(a_2d[i]==1)
		{
			printf("%d ",i);
			vNum = vNum + 1;
		}	
	}
	printf("\n");
	fflush(stdout);

	free(a_1d);
	a_1d = NULL;
	
	free(a_2d);
	a_2d = NULL;

	return NULL;
}


void* VerCover_APPROX2(void* arg) 
{
	MGraph* G = (MGraph*) arg; 
	
	int VerticeNum,EdgeNum,i,j,m;
	int vNum;

	VerticeNum = G -> numVertexes;  
	EdgeNum = G -> numEdges;
	
       /*a_2d is to store whether a node is selected as a vertex*/
	int *a_2d = (int*)malloc(sizeof(int)*VerticeNum);
	for(i=0;i<VerticeNum;i++)
	{
		a_2d[i] = 0;
	}



		for(i=0;i<VerticeNum-1;i++)
		{
			if (a_2d[i] == 1)
			{
				continue;
			}
			else			
			{
				for(j=i+1;j<VerticeNum;j++)
				{
					if (a_2d[j] == 1)
					{
						continue;
					}
					else			
					{
						if (G->arc[i][j]==1)
						{
					
							a_2d[i] = 1; //indicating that Node i is selected to be one vertex
							a_2d[j] = 1; //indicating that Node j is selected to be one vertex			
						}
					}
				}
			}	
		}

	/*to print out the vertexes*/
	vNum = 0;
	printf("APPROX-VC-2: ");
	for (i = 0; i < VerticeNum; i++)
	{
		if(a_2d[i]==1)
		{
			printf("%d ",i);
			vNum = vNum + 1;
		}			
	}
	printf("\n");
	fflush(stdout);


	free(a_2d);
	a_2d = NULL;

	return NULL;
}

void pclock(char *msg, clockid_t cid)
       {
	   struct timespec ts;
           printf("%s", msg);
	  // if (clock_gettime(cid, &ts) == -1)
            //   handle_error("clock_gettime");
           printf("%4ld.%09ld\n", ts.tv_sec, ts.tv_nsec/1000000);
	   fflush(stdout);	
       }

void pratio(char *msg, float ratio)
{
	printf("%s: ",msg);
	printf("%2.4f\n",ratio);
	fflush(stdout);	
}


int main(int argc, char** argv)
{
/*creating pipe between graphGlen and multi-thread*/
	int fds_12[2]; //data written to the file descriptor read [0] can be read back from write [1]
	int pid_rgen,pid2;
	int ix;
	char* buffer[1];
	char buf_1[1];
	char buf_2[2];
	
	
	pipe (fds_12);  

	pid_rgen = fork();  
	
	if (pid_rgen == 0) {
	 	for (ix=0;ix<10;ix++)
		{  
			pid2 = fork();
			if (pid2 == 0)
			{
				close (fds_12[0]);
				dup2 (fds_12[1],STDOUT_FILENO);
				execvp ("/home/wdietl/graphGen/graphGen",argv);
			}
		}
	}
	else
	{
		close (fds_12[1]);
		dup2 (fds_12[0],STDIN_FILENO); 			


		MGraph G;
		GraphAdjList GL;
		int i,j,s;
		int VertexNum;
		int Edge_num;
		int flag;
		int num_CNFSAT;
		int num_APPROX1;
		int num_APPROX2;
		float ratio1,ratio2;

		pthread_t thread_CNFSAT;
		pthread_t thread_APPROX1;
		pthread_t thread_APPROX2;

		clockid_t cid_CNFSAT;
		clockid_t cid_APPROX1;
		clockid_t cid_APPROX2;

		char str[10000];

	    while (fgets(str,10000,stdin))
	    {
		if (str[0] != 's')
			printf("%s",str);
			fflush(stdout);
		if (str[0]=='V')
		{
		   VertexNum = VStr(str);
		   //printf("%d \n",VertexNum);
		}
		else if (str[0]=='E')
		{
		    Edge_num = ENum(str);
		    flag = CreateMGraph(&G, Edge_num, VertexNum, str);
		    CreateALGraph(G,&GL);
		    int i_test;	    
		    if (Edge_num > 0 && flag == 1)
		    {
			for (i_test = 0;i_test<10;i_test++)
			{
			s = pthread_create (&thread_CNFSAT,NULL,&VerCover_CNFSAT,&G);
			if (s != 0)
		       		handle_error_en(s, "pthread_create");
			s = pthread_getcpuclockid(thread_CNFSAT, &cid_CNFSAT);
			if (s != 0)
		       		handle_error_en(s, "pthread_getcpuclockid");
			pthread_join(thread_CNFSAT,NULL);
			char ms1[] = "CNF-SAT-VC's CPU time:  ";
			struct timespec ts;
           		printf("%s", ms1);
			clock_gettime(cid_CNFSAT, &ts);
		//	   if (clock_gettime(cid_CNFSAT, &ts) == -1)
           	//	    handle_error("clock_gettime");
          	        printf("%4ld.%09ld\n", ts.tv_sec, ts.tv_nsec/1000);
	  	        fflush(stdout);	

			//pclock(ms1, cid_CNFSAT);
			sleep(1);
	
			s = pthread_create (&thread_APPROX1,NULL,&VerCover_APPROX1,&G);
			if (s != 0)
		       		handle_error_en(s, "pthread_create");
			s = pthread_getcpuclockid(thread_APPROX1, &cid_APPROX1);
			if (s != 0)
		       		handle_error_en(s, "pthread_getcpuclockid"); 
			pthread_join(thread_APPROX1,NULL);
			char ms2[] = "APPROX-VC-1's CPU time:  ";
			struct timespec ts1;
			printf("%s", ms2);
			clock_gettime(cid_APPROX1, &ts1);
		//	   if (clock_gettime(cid_CNFSAT, &ts) == -1)
           	//	    handle_error("clock_gettime");
          	        printf("%4ld.%09ld\n", ts1.tv_sec, ts1.tv_nsec/1000);
	  	        fflush(stdout);	

			//pclock(ms2, cid_APPROX1);
			sleep(1);
		
			s = pthread_create (&thread_APPROX2,NULL,&VerCover_APPROX2,&G);
			if (s != 0)
		       		handle_error_en(s, "pthread_create");
			s = pthread_getcpuclockid(thread_APPROX2, &cid_APPROX2);
			if (s != 0)
		       		handle_error_en(s, "pthread_getcpuclockid"); 
			char ms3[] = "APPROX-VC-2's CPU time:  ";		
			pthread_join(thread_APPROX2,NULL);	
			struct timespec ts2;
			printf("%s", ms3);
			clock_gettime(cid_APPROX2, &ts2);
		//	   if (clock_gettime(cid_CNFSAT, &ts) == -1)
           	//	    handle_error("clock_gettime");
          	        printf("%4ld.%09ld\n", ts2.tv_sec, ts2.tv_nsec/1000);
	  	        fflush(stdout);	

			//pclock(ms3, cid_APPROX2); 
			sleep(1);

			}
			/*calculate the approximate ratio*/
			/*float x1, x2, x3;
			x1 = (float) g[0].vNum;
			x2 = (float) g[1].vNum;
			x3 = (float) g[2].vNum;

			char msg1[] = "Approximation ratio of APPROX1: ";
			char msg2[] = "Approximation ratio of APPROX2: ";

			ratio1 = x2/x1;
			pratio(msg1, ratio1);
			ratio2 = x3/x1;
			pratio(msg2, ratio2);*/

			//free(g);
			//g = NULL;
		
		    
			//free(g);
			//g = NULL;
		    }
		}
		else if (str[0]=='s')
		{
		    int str_size1;
		    int str_size;
		    int s_flag;
		    
		    for (str_size = 0;str[str_size]!='\0';str_size++)
		        {
		            str_size1 = str_size;
		        }

		    s_flag = 0;

		    int flag1 = 1;

		    while(str[s_flag]!= ' ')
		    {
		        s_flag++;
		    }

		    while(str[s_flag+flag1]!=' ')
		    {
		        flag1++;
		    }

		    int s1,s2;
		    int firstNode = 0;
		    int lastNode = 0;
		    for (s1 = s_flag+1; s1 < s_flag + flag1; s1++)
		    {
		        firstNode = (int)(firstNode + (str[s1]-'0') * pow(10,s_flag + flag1-1-s1));
		    }

		    for (s2 = s_flag + flag1 + 1; s2 < str_size1;s2++)
		    {
		        lastNode = (int)(lastNode + (str[s2]-'0') * pow(10,str_size1-1-s2));
	//                printf("%d %d %d\n",s2,str[s2]-'0',str_size1-1-s2);
		    }
	//            printf("%d %d\n",firstNode,lastNode);

		    if(firstNode>=VertexNum || lastNode>=VertexNum)
		    {
		        printf("Error: one or more vertexes do not exist\n");
		        continue;
		    }

		    if(firstNode == lastNode)
		    {
		        printf("%d-%d\n",firstNode,lastNode);		
		        continue;
		    }


		    CreateALGraph(G,&GL);
		    BFSTraverse(GL,firstNode,lastNode);
	//	    VerCover(G, VertexNum); 
		}
	    }
		return 0;
	}
}
