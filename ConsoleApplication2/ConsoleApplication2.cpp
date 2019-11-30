#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX 10
#define INFINIT 32767
#define NumMaxChar 10
#define NumMAXTN 10 


///////////////////////////////数据结构定义////////////////////////////////// 

//////////////////////////NFA图结构//////////////////////////////////
typedef struct edge
{//边
	int dest;
	char cost;
	struct edge* link;	//指向下一边
}*Edge;

typedef struct vertex
{//顶点
	char data;	//状态
	Edge adj;	//边
}*Vertex;

typedef struct graph
{//图
	Vertex NodeTable;
	int NumVertex;
	int MaxNumVertex;
	int NumEdge;
}*Graph;
//////////////////////////////////////////////////////////////////////
//////////////////////////状态转换表机构//////////////////////////////
typedef struct tablenode
{//转换表节点
	char newname;//新命名
	char ch[MAX];//顶点集合
}*TableNode;

typedef struct tablequeue
{//转换表列
	TableNode TN[MAX];//转换表节点数组
	char transword;//转换条件
	int NumTn;//添加的顶点数
}*TableQueue;

typedef struct transmatrix
{//状态转换矩阵
	TableQueue TQ;//转换表列组
	int transnum;//转换表列数
}*TranMatrix;

///////////////////////////////////////////////////////////////////////////// 


///////////////////////////////////图操作////////////////////////////////////  
int GraphEmpty(Graph g)
{//图判空
	return g->NumVertex == 0;
}

int GraphFull(Graph g)
{//判图满
	return g->NumVertex == g->MaxNumVertex;
}

char GetValue(Graph g, int i)
{//寻找下标为i的顶点
	return (i > 0 && i < g->NumVertex ? g->NodeTable[i].data : ' ');
}


void Insert_Vertex(Graph g, char vertex)
{//插入新的顶点
	g->NodeTable[g->NumVertex].data = vertex;
	g->NodeTable[g->NumVertex].adj = NULL;
	g->NumVertex++;
}

void Insert_Edge(Graph g, int v1, int v2, char weight)
{//插入边
	Edge p;
	p = (Edge)malloc(sizeof(struct edge));
	p->cost = weight;
	p->dest = v2;
	p->link = g->NodeTable[v1].adj;
	g->NodeTable[v1].adj = p;
}

int GetVertexPos(Graph g, char v)
{//得到顶点在图中的下标
	int i = 0;
	while (i < g->NumVertex)
	{
		if (g->NodeTable[i].data == v)
			return i;
		i++;
	}
	return INFINIT;
}

void Construct_Graph(Graph g)
{//创建图
	int k, j, i, vexn, edgen;
	char head, tail, name;
	char weight;
	g->NumVertex = 0;
	g->NumEdge = 0;
	g->MaxNumVertex = MAX;
	g->NodeTable = (Vertex)malloc((g->MaxNumVertex) * sizeof(struct vertex));
	printf("输入NFA状态数:");
	cin >> vexn;// scanf("%d", &vexn);
	printf("输入NFA状态名称:\n");
	_flushall();
	//依次获取状态名称，并将这些顶点插入图中
	for (i = 0; i < vexn; i++)
	{
		cin >> name;// scanf("%c", &name);
		_flushall();
		Insert_Vertex(g, name);
	}
	printf("输入NFA的边数:");
	cin >> edgen;//	scanf("%d", &edgen);
	printf("输入 起始状态，接受字符 和 到达状态:\n");
	_flushall();
	//依次获取边的信息（起始顶点，接受字符，到达的顶点），并将这些边插入图中
	for (i = 0; i < edgen; i++)
	{
		_flushall();
		cin >> tail >> weight >> head;// scanf("%c %c %c", &tail, &weight, &head);
		k = GetVertexPos(g, tail);
		j = GetVertexPos(g, head);
		Insert_Edge(g, k, j, weight);
	}
}

void Destruct_Graph(Graph g)
{//销毁图  
	int i;
	Edge p;
	for (i = 0; i < g->NumVertex; i++)
	{
		p = g->NodeTable[i].adj;
		while (p != NULL)
		{
			g->NodeTable[i].adj = p->link;
			p->link = NULL;
			free(p);
			p = g->NodeTable[i].adj;
		}
	}
	g->NumEdge = 0;
	g->NumVertex = 0;
	printf("图已销毁\n");
}


void Show_Graph(Graph g)
{//显示图   
	int i;
	Edge p;
	for (i = 0; i < g->NumVertex; i++)
	{
		p = g->NodeTable[i].adj;
		while (p != NULL)
		{
			printf("(%c,%c):%c  ", g->NodeTable[i].data, g->NodeTable[p->dest].data, p->cost);
			p = p->link;
		}
		printf("\n");
	}
}
//////////////////////////////////////////////////////////////////////////// 


////////////////////////////初始化状态转换矩阵//////////////////////////////
void Init_Matrix(TranMatrix TM)
{
	int i, j, k;
	printf("输入接受字符数:");
	cin >> TM->transnum;// scanf("%d", &TM->transnum);
	TM->TQ = (TableQueue)malloc((TM->transnum + 1) * sizeof(struct tablequeue));
	//初始化转换矩阵的第一列
	for (j = 0; j < MAX; j++)
	{
		TM->TQ[0].TN[j] = (TableNode)malloc(sizeof(struct tablenode));
		TM->TQ[0].TN[j]->newname = '\0';
		for (k = 0; k < NumMaxChar; k++)
			TM->TQ[0].TN[j]->ch[k] = '\0';
	}
	TM->TQ[0].transword = 'I';
	TM->TQ[0].NumTn = 0;
	//初始化转换矩阵的其它列
	for (i = 1; i <= TM->transnum; i++)
	{
		printf("输入第%d个转换字符:\n", i);
		_flushall();
		cin >> TM->TQ[i].transword;// scanf("%c", &TM->TQ[i].transword);
		for (j = 0; j < MAX; j++)
		{
			TM->TQ[i].TN[j] = (TableNode)malloc(sizeof(struct tablenode));
			TM->TQ[i].TN[j]->newname = '\0';
			for (k = 0; k < NumMaxChar; k++)
				TM->TQ[i].TN[j]->ch[k] = '\0';
		}
		TM->TQ[i].NumTn = 0;
	}
}
//////////////////////////////////////////////////////////////////////////// 


////////////////////////////状态转换操作////////////////////////////////////
void BubbleSort(char a[], int len)
{//冒泡排序
	int i, j;
	char temp;

	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
			}
		}
	}
}


int CheckChar(char t[], char ti)//检查数组t中是否有与ti相同的字符
{//0 有重复，1 无重复
	int i;
	for (i = 0; i < NumMaxChar; i++)
	{
		if (t[i] == ti) return 0;
	}
	return 1;
}

int CheckTable(TranMatrix TM, char str[])//检查转换矩阵的第一列中是否有与str相同的字符串
{// i 有重复（并返回重复的位置），1 无重复
	int i;
	for (i = 0; i < TM->TQ[0].NumTn; i++)
	{
		if (!strcmp(TM->TQ[0].TN[i]->ch, str))
			return i;
	}
	return 1;
}


void Smove(Graph g, char t1[], char t2[], char transword)
{//根据NFA，将t1中的状态接受了transword后转为对应的t2中的状态
	int i, j, k = 0, check, len;
	Edge p;
	while (t2[k] != '\0') k++;
	for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < g->NumVertex; j++)
		{
			if (g->NodeTable[j].data == t1[i])
			{
				p = g->NodeTable[j].adj;
				while (p != NULL)
				{
					if (p->cost == transword)
					{
						check = CheckChar(t2, g->NodeTable[p->dest].data);
						if (check == 1)
						{
							t2[k] = g->NodeTable[p->dest].data;
							k++;
						}
						p = p->link;
					}
					else
						p = p->link;
				}
			}
		}
	}
	len = k;
	BubbleSort(t2, k);
}

void E_Closure(Graph g, char t[])
{//对进行了Smove后的状态集合求闭包，并将结果按升序排列
	int i = 0, j, k = 0, check, len;
	Edge p;
	//找到字符数组中的最后一个字符的位置
	while (t[k] != '\0') k++;

	for (i = 0; t[i] != '\0'; i++)
	{ 	//每个顶点都要查看一次
		for (j = 0; j < g->NumVertex; j++)
		{	//
			if (g->NodeTable[j].data == t[i])
			{
				p = g->NodeTable[j].adj;
				while (p != NULL)
				{
					if (p->cost == '*')	//空输入
					{
						check = CheckChar(t, g->NodeTable[p->dest].data);
						if (check == 1)
						{
							t[k] = g->NodeTable[p->dest].data;
							k++;
						}
					}
					p = p->link;
				}
			}
		}
	}
	len = k;
	BubbleSort(t, k);	//结果按升序排列
}


void Show_TranMatrix(TranMatrix TM)
{
	int i, j, k;
	printf("状态转换矩阵:\n");
	for (j = 0; j <= TM->transnum; j++)
		printf("转换字符:%c      ", TM->TQ[j].transword);
	printf("\n");
	for (k = 0; k < MAX; k++)
	{
		for (j = 0; j <= TM->transnum; j++)
		{
			if (TM->TQ[j].TN[k]->newname == '\0') printf("NULL ");
			else printf("%c:", TM->TQ[j].TN[k]->newname);

			for (i = 0; TM->TQ[j].TN[k]->ch[i] != '\0'; i++)
			{
				printf("%c", TM->TQ[j].TN[k]->ch[i]);
			}

			if (TM->TQ[j].TN[k]->ch[0] == '\0') printf("NULL");
			printf("           ");
		}
		printf("\n");
	}
}


void NFA_to_DFA(Graph g, TranMatrix TM)
{
	int j = 0, i, check;
	char temp[2] = { '\0','\0' };
	//用开始顶点初始化第一列第一个节点//////////////////////////////
	TM->TQ[0].TN[0]->ch[0] = g->NodeTable[0].data;

	for (i = 1; i <= TM->transnum; i++)
		E_Closure(g, TM->TQ[0].TN[0]->ch);

	printf("输入新状态名:");
	_flushall();
	cin >> TM->TQ[0].TN[0]->newname;// scanf("%c", &TM->TQ[0].TN[0]->newname);

	TM->TQ[0].NumTn++;
	while (j < TM->TQ[0].NumTn)
	{
		for (i = 1; i <= TM->transnum; i++)
		{
			Smove(g, TM->TQ[0].TN[j]->ch, TM->TQ[i].TN[j]->ch, TM->TQ[i].transword);
			E_Closure(g, TM->TQ[i].TN[j]->ch);

			if (TM->TQ[i].TN[j]->ch[0] != '\0')	//如果为空则可忽略，不记入结果状态
			{
				check = CheckTable(TM, TM->TQ[i].TN[j]->ch);
				if (check == 1)
				{
					printf("输入新状态名:");
					_flushall();
					cin >> TM->TQ[i].TN[j]->newname; // scanf("%c", &TM->TQ[i].TN[j]->newname);
					strcpy(TM->TQ[0].TN[TM->TQ[0].NumTn]->ch, TM->TQ[i].TN[j]->ch);
					TM->TQ[0].TN[TM->TQ[0].NumTn]->newname = TM->TQ[i].TN[j]->newname;
					TM->TQ[0].NumTn++;
				}
				else TM->TQ[i].TN[j]->newname = TM->TQ[0].TN[check]->newname;
			}
		}
		j++;
	}
}

void Show_DFA(TranMatrix TM)
{
	int i, j;
	for (j = 0; TM->TQ[0].TN[j]->newname != '\0'; j++)
	{
		for (i = 1; i <= TM->transnum; i++)
		{
			if (TM->TQ[i].TN[j]->newname != '\0')
				printf("(%c,%c,%c)   ", TM->TQ[0].TN[j]->newname, TM->TQ[i].transword, TM->TQ[i].TN[j]->newname);
		}
		printf("\n");
	}
}


int main() {
	Graph g;
	TranMatrix TM;
	g = (Graph)malloc(sizeof(struct graph));
	TM = (TranMatrix)malloc(sizeof(struct transmatrix));
	Construct_Graph(g);
	Init_Matrix(TM);
	printf("\nNFA图:\n");
	Show_Graph(g);
	printf("\n(初始状态):\n");
	Show_TranMatrix(TM);
	NFA_to_DFA(g, TM);
	printf("\n(转换过程):\n");
	Show_TranMatrix(TM);
	printf("\n转化后的DFA:\n");
	Show_DFA(TM);
	printf("\n程序结束，销毁图...\n");
	Destruct_Graph(g);
	Show_Graph(g);
}
