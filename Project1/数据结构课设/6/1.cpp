#define _CRT_SECURE_NO_WARNINGS 1


/*
6�������޽� (����) ��ͼ��
[��������]
����A����n����ͨ��Ŧ������1�ź�n�ŷǳ���Ҫ��Ϊ�˼�ǿ����������A�о�����1�ŵ�n����Ŧ���޽�һ��������
���������ɺܶ�������ɣ�ÿ���������������ͨ��Ŧ��������̽����m�������Ϊ��ѡ��������ͨ��Ŧ֮�����ֻ��һ����ѡ�������û���������������ͬһ����ͨ��Ŧ��
����������n�����ʩ���Ĺ�˾��ÿ�κ�ѡ�����ֻ����һ����˾ʩ����ÿ�ҹ�˾ʩ����Ҫ������һ�¡���ÿ�ҹ�˾���ֻ���޽�һ����ѡ��������й�˾ͬʱ��ʼʩ����
������Ϊ��Ŀ�����ˣ������˺�ѡ�������Ϣ����������԰��Լ����뷨ѡ��һ�����������ʩ���������޽���������������Ҫ�����졣
�����ʽ
��������ĵ�һ�а�����������n, m����һ���ո�ָ����ֱ��ʾ��ͨ��Ŧ�������ͺ�ѡ�����������
��2�е���m+1�У�ÿ�а�����������a, b, c����ʾ��Ŧa����Ŧb֮������޽�һ���������Ҫ��ʱ��Ϊc�졣
[����Ҫ��]
�����ʽ
�������һ���������޽�����������·������Ҫ��������
��������
6 6
1 2 4
2 3 4
3 6 7
1 4 2
4 5 5
5 6 6
�������
6
����˵��
���������޽�����·�����֡�
������һ�־�������Ŧ����Ϊ1, 2, 3, 6������Ҫ��ʱ��ֱ���4, 4, 7����������������Ҫ7�����ꣻ
�����ڶ��־�������Ŧ����Ϊ1, 4, 5, 6������Ҫ��ʱ��ֱ���2, 5, 6����������������Ҫ6�����ꡣ
    �ڶ��ַ������õ��������١�

*/


#include <iostream>
#include <algorithm>
using namespace std;

const int MAX_M = 10000; //����ѡ�������
const int MAX_N = 1000;  //���ͨ��Ŧ����

struct Edge 
{
    int u, v;    //������Ŧ
    int w;       //Ȩ�أ�������ʱ��
};

Edge edges[MAX_M];
int parent[MAX_N + 1];
int TreeRank[MAX_N + 1];
int n, m;

int find(int x) 
{
    if (parent[x] != x)
    {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}
//���Ҹ��ڵ㣬��·��ѹ��


bool unionSets(int x, int y)
{
    int rootX = find(x), rootY = find(y);

    if (rootX == rootY)
    {
        return false;
    }

    if (TreeRank[rootX] > TreeRank[rootY])
    {
        parent[rootY] = rootX;
    }
    else if (TreeRank[rootX] < TreeRank[rootY])
    {
        parent[rootX] = rootY;
    }
    else 
    {
        parent[rootY] = rootX;
        TreeRank[rootX]++;
    }
    return true;
}
//�ϲ���������


bool canBuildMetro(int maxTime)
{
    
    for (int i = 1; i <= n; i++)
    {
        parent[i] = i;
        TreeRank[i] = 0;
    }
    //��ʼ��

    for (int i = 0; i < m; i++) 
    {
        if (edges[i].w <= maxTime) 
        {
            unionSets(edges[i].u, edges[i].v);      //�������бߣ���ʩ��ʱ��<=maxTime�ıߺϲ�
        }
    }
    

    return find(1) == find(n);               //�ж�1��n�Ƿ���ͨ
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }


    sort(edges, edges + m, [](const Edge& a, const Edge& b){return a.w < b.w;});        //��ʩ��ʱ������

    
    int left = 0, right = edges[m - 1].w, result = -1;
    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (canBuildMetro(mid)) 
        {
            result = mid;
            right = mid - 1;
        }
        else 
        {
            left = mid + 1;
        }
    }
    //���ֲ�����Сʩ��ʱ��

    cout << result << endl;
    return 0;
}
