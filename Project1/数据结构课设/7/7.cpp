#define _CRT_SECURE_NO_WARNINGS 1

/*
7、公交线路提示  (必做) （图）
[问题描述]
上网下载真实南京公交线路图，建立南京主要公交线路图的存储结构。
[基本要求]
（1）输入任意两站点，给出转车次数最少的乘车路线。
（2）输入任意两站点，给出经过站点最少的乘车路线。

*/


#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

#define MAX_EDGES 200
#define MAX_STATIONS 20000

//数据结构定义
struct Edge 
{
    int to;       // 目标站点
    int lineID;   // 所属公交线路ID
};


struct Node
{
    int stationID;
    Edge edges[MAX_EDGES];
    int edgecount;
}graph[MAX_STATIONS];



int graphsize = 0;                //全局变量，站点个数

int FindStationIndex(int stationID)
{
    for (int i = 0; i < graphsize; i++)
    {
        if (graph[i].stationID == stationID)
        {
            return i;
        }
    }
    return -1;
}

void AddEdge(int fromIndex, int toStationID, int lineID)
{
    int curredgecount = graph[fromIndex].edgecount;
    graph[fromIndex].edges[curredgecount].to = toStationID;
    graph[fromIndex].edges[curredgecount].lineID = lineID;

    graph[fromIndex].edgecount++;
}

void CreateGraph(const string& filename)
{
    fstream infile(filename);
    if (!infile.is_open())
    {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    string line;
    int prevStationID = -1;
    int prevLineID = -1;

    while (getline(infile, line))
    {
        int lineID, orderID, stationID;
        sscanf(line.c_str(), "%d %d %d", &lineID, &orderID, &stationID);

        int stationIndex = FindStationIndex(stationID);

        if (stationIndex == -1)
        {
            stationIndex = graphsize;
            graph[graphsize].stationID = stationID;
            graph[graphsize].edgecount = 0;
            graphsize++;
        }

        if (prevStationID != -1 && prevLineID == lineID)
        {
            int prevIndex = FindStationIndex(prevStationID);
            AddEdge(prevIndex, stationID, lineID);
            AddEdge(stationIndex, prevStationID, lineID);   //双向互通
        }
        //同一条线直接加边

        prevStationID = stationID;
        prevLineID = lineID;
    }
    infile.close();
}


int FindShortestPath(int startID, int endID, int path[])
{
    int prevStation[MAX_STATIONS];
    int visited[MAX_STATIONS] = { 0 };    //访问数组初始化

    int queue[MAX_STATIONS];
    int front = 0;
    int rear = 0;                         //BFS队列

    int startIndex = FindStationIndex(startID);
    queue[rear] = startIndex;
    rear++;
    visited[startIndex] = 1;

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;

        if (graph[currentIndex].stationID == endID)
        {
            break;
        }

        Node& currentNode = graph[currentIndex];
        for (int i = 0; i < currentNode.edgecount; i++)
        {
            Edge& curredge = currentNode.edges[i];
            int nextIndex = FindStationIndex(curredge.to);     //找到边所指向的站点的索引
            
            if (!visited[nextIndex])
            {
                visited[nextIndex] = 1;                       //标记访问
                prevStation[nextIndex] = currentIndex;        //记录前驱
                queue[rear] = nextIndex;
                rear++;
            }
        }
    }

    int endIndex = FindStationIndex(endID);
    int pathlength = 0;

    for (int at = endIndex; at != startIndex; at = prevStation[at])
    {
        path[pathlength] = graph[at].stationID;
        pathlength++;
    }
    path[pathlength] = graph[startIndex].stationID;
    pathlength++;

    cout << "最少经过" << pathlength - 1 << "站" << endl;

    return pathlength;
}
//最短路径

void printpath(int path[], int length)
{
    for (int i = length-1; i >= 0; i--)
    {
        if (i < length-1)
        {
            printf("->");
        }
        printf("%d", path[i]);
    }
    printf("\n");
}
//倒叙输出


int FindMinTransfersPath(int startID, int endID, int path[])
{
    int transfers[MAX_STATIONS];
    int prevStation[MAX_STATIONS];
    int prevLine[MAX_STATIONS];
    int visited[MAX_STATIONS] = { 0 };    //访问数组初始化


    for (int i = 0; i < graphsize; i++)
    {
        transfers[i] = INT_MAX;           //初始化每个站点的转车次数为最大
    }

    int startIndex = FindStationIndex(startID);

    int queue[MAX_STATIONS];
    int front = 0;
    int rear = 0;                   //BFS队列

    queue[rear] = startIndex;
    rear++;
    visited[startIndex] = 1;

    transfers[startIndex] = 0;
    prevLine[startIndex] = -1;

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;
        visited[currentIndex] = 1;                                //标记访问


        Node& currentNode = graph[currentIndex];
        for (int i = 0; i < currentNode.edgecount; i++)
        {
            Edge& curredge = currentNode.edges[i];
            int nextIndex = FindStationIndex(curredge.to);
            int newtransfers = transfers[currentIndex] + (prevLine[currentIndex] == curredge.lineID ? 0 : 1);

            if (newtransfers < transfers[nextIndex])
            {
                transfers[nextIndex] = newtransfers;              //更新换乘次数
                prevStation[nextIndex] = currentIndex;            //更新最小换乘前驱站点
                prevLine[nextIndex] = curredge.lineID;            //更新最小换乘前驱线路

                if (!visited[nextIndex])
                {
                    visited[nextIndex] = 1;                       //标记访问
                    queue[rear] = nextIndex;
                    rear++;
                }
            }
        }
    }

    int endIndex = FindStationIndex(endID);
    int pathlength = 0;

    for (int at = endIndex; at != startIndex; at = prevStation[at])
    {
        path[pathlength] = graph[at].stationID;
        pathlength++;
    }
    path[pathlength] = graph[startIndex].stationID;
    pathlength++;

    cout << "最少换乘" << transfers[endIndex] - 1 << "次" << endl;

    return pathlength;
}
//最少换乘


int main()
{
    // 构建图
    CreateGraph("3_南京市公交线路_17270条.txt");

    int start, end;
    cout << "输入起点站和终点站: ";
    cin >> start >> end;

    int startIndex = FindStationIndex(start);
    int endIndex = FindStationIndex(end);

    if (startIndex == -1 || endIndex == -1)
    {
        cerr << "起点或终点无效！" << endl;
        return -1;
    }

    // 最少换乘次数
    printf("最少换乘次数的路线: \n");
    int minTransfersPath[MAX_STATIONS];
    int transfersLength = FindMinTransfersPath(start, end, minTransfersPath);
    printpath(minTransfersPath, transfersLength);

    // 最少站点数
    cout << "最少站点数的路线: \n";
    int ShortestPath[MAX_STATIONS];
    int ShortestLength = FindShortestPath(start, end, ShortestPath);
    printpath(ShortestPath, ShortestLength);
    return 0;
}

