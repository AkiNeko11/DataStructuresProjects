#define _CRT_SECURE_NO_WARNINGS 1
/*
【19】最小生成树 (选做) （图）
[问题描述]
利用普利姆算法和克鲁斯卡尔算法实现最小生成树问题。
[基本要求]
（1）自行建立图的数据文件，第一行是顶点个数，然后依次是顶点名，接下来是边，用float表示边的权值；             
（2）以邻接表或者邻接矩阵表示图皆可；
（3）分别利用Prim和Kruskal算法实现最小生成树；
（4）输出最小生成树的权值之和，及所用的边。

*/

#include<iostream>
#include<stdio.h>
#include<fstream>
#include<algorithm>
using namespace std;

#define MAX_V 20

struct node
{
	int code;
};

struct graph
{
	int vexnum;
	int arcnum;
	node vex[MAX_V + 1];
	float arcs[MAX_V + 1][MAX_V + 1];
};

void create(graph& G, FILE* fp)
{
	fscanf(fp, "%d", &G.vexnum);                     //顶点个数

	for (int i = 1; i <= G.vexnum; i++)
	{
		fscanf(fp, "%d", &G.vex[i].code);
	}
	//顶点的名字编号


	fscanf(fp, "%d", &G.arcnum);                     //边个数

	for (int i = 1; i <= G.vexnum; i++)
	{
		for (int j = 1; j <= G.vexnum; j++)
		{
			G.arcs[i][j] = INT_MAX;
		}
	}

	for (int k = 0; k < G.arcnum; k++)
	{
		int u, v;
		float w;
		fscanf(fp, "%d %d %f", &u, &v, &w);

		G.arcs[u][v] = w;
		G.arcs[v][u] = w;
	}
}

struct
{
	float lowcost;
	int vexcode;
}closest[MAX_V + 1];


void Prim_mintree(graph G, int v)
{
	for (int i = 1; i <= G.vexnum; i++)
	{
		closest[i].vexcode = v;
		closest[i].lowcost = G.arcs[i][v];
	}
	closest[v].lowcost = 0;

	float totalweight = 0;

	cout << "Prim算法最小生成树的边:" << endl;

	for (int i = 2; i <= G.vexnum; i++)
	{
		int k = -1;
		float min = INT_MAX;

		for (int j = 1; j <= G.vexnum; j++)
		{
			if (closest[j].lowcost != 0 && closest[j].lowcost < min)
			{
				k = j;
				min = closest[j].lowcost;
			}
		}

		totalweight += min;

		cout << "(" << closest[k].vexcode << " " << k << " " << min << ")" << endl;

		closest[k].lowcost = 0;

		for (int j = 1; j <= G.vexnum; j++)
		{
			if (G.arcs[k][j] < closest[j].lowcost)
			{
				closest[j].lowcost = G.arcs[k][j];
				closest[j].vexcode = k;
			}
		}
	}

	cout << "权值之和: " << totalweight << endl;
}
//prim算法



struct Edge 
{
	int u, v;
	float weight;
};

int find(int parent[], int x) 
{
	if (parent[x] != x) {
		parent[x] = find(parent, parent[x]);
	}
	return parent[x];
}

void unionSet(int parent[], int rank[], int x, int y) 
{
	int rootX = find(parent, x);
	int rootY = find(parent, y);

	if (rootX != rootY) {
		if (rank[rootX] > rank[rootY]) 
		{
			parent[rootY] = rootX;
		}
		else if (rank[rootX] < rank[rootY]) 
		{
			parent[rootX] = rootY;
		}
		else 
		{
			parent[rootY] = rootX;
			rank[rootX]++;
		}
	}
}

void Kruskal(graph& G) 
{
	// 构建边列表
	Edge edges[MAX_V * MAX_V];
	int edgeCount = 0;

	for (int i = 1; i <= G.vexnum; i++) 
	{
		for (int j = i + 1; j <= G.vexnum; j++) 
		{ // 无向图，只需要遍历一半
			if (G.arcs[i][j] != INT_MAX) 
			{
				edges[edgeCount] = { i, j, G.arcs[i][j] };
				edgeCount++;
			}
		}
	}

	// 按权值升序排序边
	sort(edges, edges + edgeCount, [](Edge a, Edge b) 
		{
		return a.weight < b.weight;
		});

	int parent[MAX_V + 1], rank[MAX_V + 1];

	for (int i = 1; i <= G.vexnum; i++) 
	{
		parent[i] = i;
		rank[i] = 0;
	}

	float totalWeight = 0;
	int edgeUsed = 0;

	cout << "Kruskal最小生成树的边:" << endl;

	for (int i = 0; i < edgeCount && edgeUsed < G.vexnum - 1; i++) {
		int u = edges[i].u;
		int v = edges[i].v;
		float weight = edges[i].weight;

		if (find(parent, u) != find(parent, v)) { // 不形成环
			unionSet(parent, rank, u, v);
			totalWeight += weight;
			edgeUsed++;
			cout << "(" << u << " " << v << " " << weight << ")" << endl;
		}
	}

	cout << "权值之和: " << totalWeight << endl;
}


int main()
{
	FILE* fp = NULL;
	fp = fopen("graph.txt", "r");
	if (!fp)
	{
		cout << "fail to open" << endl;
		exit(0);
	}

	graph G;

	create(G, fp);

	cout << "prim算法:" << endl;
	Prim_mintree(G, 1);

	cout << endl;

	cout << "Kruskal算法:" << endl;
	Kruskal(G);
}