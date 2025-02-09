#define _CRT_SECURE_NO_WARNINGS 1
/*
��19����С������ (ѡ��) ��ͼ��
[��������]
��������ķ�㷨�Ϳ�³˹�����㷨ʵ����С���������⡣
[����Ҫ��]
��1�����н���ͼ�������ļ�����һ���Ƕ��������Ȼ�������Ƕ��������������Ǳߣ���float��ʾ�ߵ�Ȩֵ��             
��2�����ڽӱ�����ڽӾ����ʾͼ�Կɣ�
��3���ֱ�����Prim��Kruskal�㷨ʵ����С��������
��4�������С��������Ȩֵ֮�ͣ������õıߡ�

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
	fscanf(fp, "%d", &G.vexnum);                     //�������

	for (int i = 1; i <= G.vexnum; i++)
	{
		fscanf(fp, "%d", &G.vex[i].code);
	}
	//��������ֱ��


	fscanf(fp, "%d", &G.arcnum);                     //�߸���

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

	cout << "Prim�㷨��С�������ı�:" << endl;

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

	cout << "Ȩֵ֮��: " << totalweight << endl;
}
//prim�㷨



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
	// �������б�
	Edge edges[MAX_V * MAX_V];
	int edgeCount = 0;

	for (int i = 1; i <= G.vexnum; i++) 
	{
		for (int j = i + 1; j <= G.vexnum; j++) 
		{ // ����ͼ��ֻ��Ҫ����һ��
			if (G.arcs[i][j] != INT_MAX) 
			{
				edges[edgeCount] = { i, j, G.arcs[i][j] };
				edgeCount++;
			}
		}
	}

	// ��Ȩֵ���������
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

	cout << "Kruskal��С�������ı�:" << endl;

	for (int i = 0; i < edgeCount && edgeUsed < G.vexnum - 1; i++) {
		int u = edges[i].u;
		int v = edges[i].v;
		float weight = edges[i].weight;

		if (find(parent, u) != find(parent, v)) { // ���γɻ�
			unionSet(parent, rank, u, v);
			totalWeight += weight;
			edgeUsed++;
			cout << "(" << u << " " << v << " " << weight << ")" << endl;
		}
	}

	cout << "Ȩֵ֮��: " << totalWeight << endl;
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

	cout << "prim�㷨:" << endl;
	Prim_mintree(G, 1);

	cout << endl;

	cout << "Kruskal�㷨:" << endl;
	Kruskal(G);
}