#define _CRT_SECURE_NO_WARNINGS 1


/*
6、地铁修建 (必做) （图）
[问题描述]
　　A市有n个交通枢纽，其中1号和n号非常重要，为了加强运输能力，A市决定在1号到n号枢纽间修建一条地铁。
　　地铁由很多段隧道组成，每段隧道连接两个交通枢纽。经过勘探，有m段隧道作为候选，两个交通枢纽之间最多只有一条候选的隧道，没有隧道两端连接着同一个交通枢纽。
　　现在有n家隧道施工的公司，每段候选的隧道只能由一个公司施工，每家公司施工需要的天数一致。而每家公司最多只能修建一条候选隧道。所有公司同时开始施工。
　　作为项目负责人，你获得了候选隧道的信息，现在你可以按自己的想法选择一部分隧道进行施工，请问修建整条地铁最少需要多少天。
输入格式
　　输入的第一行包含两个整数n, m，用一个空格分隔，分别表示交通枢纽的数量和候选隧道的数量。
第2行到第m+1行，每行包含三个整数a, b, c，表示枢纽a和枢纽b之间可以修建一条隧道，需要的时间为c天。
[基本要求]
输出格式
　　输出一个整数，修建整条地铁线路最少需要的天数。
样例输入
6 6
1 2 4
2 3 4
3 6 7
1 4 2
4 5 5
5 6 6
样例输出
6
样例说明
　　可以修建的线路有两种。
　　第一种经过的枢纽依次为1, 2, 3, 6，所需要的时间分别是4, 4, 7，则整条地铁线需要7天修完；
　　第二种经过的枢纽依次为1, 4, 5, 6，所需要的时间分别是2, 5, 6，则整条地铁线需要6天修完。
    第二种方案所用的天数更少。

*/


#include <iostream>
#include <algorithm>
using namespace std;

const int MAX_M = 10000; //最大候选隧道数量
const int MAX_N = 1000;  //最大交通枢纽数量

struct Edge 
{
    int u, v;    //两个枢纽
    int w;       //权重，即所需时间
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
//查找父节点，并路径压缩


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
//合并两个集合


bool canBuildMetro(int maxTime)
{
    
    for (int i = 1; i <= n; i++)
    {
        parent[i] = i;
        TreeRank[i] = 0;
    }
    //初始化

    for (int i = 0; i < m; i++) 
    {
        if (edges[i].w <= maxTime) 
        {
            unionSets(edges[i].u, edges[i].v);      //遍历所有边，按施工时间<=maxTime的边合并
        }
    }
    

    return find(1) == find(n);               //判断1和n是否连通
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }


    sort(edges, edges + m, [](const Edge& a, const Edge& b){return a.w < b.w;});        //按施工时间排序

    
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
    //二分查找最小施工时间

    cout << result << endl;
    return 0;
}
