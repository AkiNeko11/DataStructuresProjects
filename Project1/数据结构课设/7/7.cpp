#define _CRT_SECURE_NO_WARNINGS 1

/*
7��������·��ʾ  (����) ��ͼ��
[��������]
����������ʵ�Ͼ�������·ͼ�������Ͼ���Ҫ������·ͼ�Ĵ洢�ṹ��
[����Ҫ��]
��1������������վ�㣬����ת���������ٵĳ˳�·�ߡ�
��2������������վ�㣬��������վ�����ٵĳ˳�·�ߡ�

*/


#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

#define MAX_EDGES 200
#define MAX_STATIONS 20000

//���ݽṹ����
struct Edge 
{
    int to;       // Ŀ��վ��
    int lineID;   // ����������·ID
};


struct Node
{
    int stationID;
    Edge edges[MAX_EDGES];
    int edgecount;
}graph[MAX_STATIONS];



int graphsize = 0;                //ȫ�ֱ�����վ�����

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
        cerr << "�޷����ļ�: " << filename << endl;
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
            AddEdge(stationIndex, prevStationID, lineID);   //˫��ͨ
        }
        //ͬһ����ֱ�Ӽӱ�

        prevStationID = stationID;
        prevLineID = lineID;
    }
    infile.close();
}


int FindShortestPath(int startID, int endID, int path[])
{
    int prevStation[MAX_STATIONS];
    int visited[MAX_STATIONS] = { 0 };    //���������ʼ��

    int queue[MAX_STATIONS];
    int front = 0;
    int rear = 0;                         //BFS����

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
            int nextIndex = FindStationIndex(curredge.to);     //�ҵ�����ָ���վ�������
            
            if (!visited[nextIndex])
            {
                visited[nextIndex] = 1;                       //��Ƿ���
                prevStation[nextIndex] = currentIndex;        //��¼ǰ��
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

    cout << "���پ���" << pathlength - 1 << "վ" << endl;

    return pathlength;
}
//���·��

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
//�������


int FindMinTransfersPath(int startID, int endID, int path[])
{
    int transfers[MAX_STATIONS];
    int prevStation[MAX_STATIONS];
    int prevLine[MAX_STATIONS];
    int visited[MAX_STATIONS] = { 0 };    //���������ʼ��


    for (int i = 0; i < graphsize; i++)
    {
        transfers[i] = INT_MAX;           //��ʼ��ÿ��վ���ת������Ϊ���
    }

    int startIndex = FindStationIndex(startID);

    int queue[MAX_STATIONS];
    int front = 0;
    int rear = 0;                   //BFS����

    queue[rear] = startIndex;
    rear++;
    visited[startIndex] = 1;

    transfers[startIndex] = 0;
    prevLine[startIndex] = -1;

    while (front < rear)
    {
        int currentIndex = queue[front];
        front++;
        visited[currentIndex] = 1;                                //��Ƿ���


        Node& currentNode = graph[currentIndex];
        for (int i = 0; i < currentNode.edgecount; i++)
        {
            Edge& curredge = currentNode.edges[i];
            int nextIndex = FindStationIndex(curredge.to);
            int newtransfers = transfers[currentIndex] + (prevLine[currentIndex] == curredge.lineID ? 0 : 1);

            if (newtransfers < transfers[nextIndex])
            {
                transfers[nextIndex] = newtransfers;              //���»��˴���
                prevStation[nextIndex] = currentIndex;            //������С����ǰ��վ��
                prevLine[nextIndex] = curredge.lineID;            //������С����ǰ����·

                if (!visited[nextIndex])
                {
                    visited[nextIndex] = 1;                       //��Ƿ���
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

    cout << "���ٻ���" << transfers[endIndex] - 1 << "��" << endl;

    return pathlength;
}
//���ٻ���


int main()
{
    // ����ͼ
    CreateGraph("3_�Ͼ��й�����·_17270��.txt");

    int start, end;
    cout << "�������վ���յ�վ: ";
    cin >> start >> end;

    int startIndex = FindStationIndex(start);
    int endIndex = FindStationIndex(end);

    if (startIndex == -1 || endIndex == -1)
    {
        cerr << "�����յ���Ч��" << endl;
        return -1;
    }

    // ���ٻ��˴���
    printf("���ٻ��˴�����·��: \n");
    int minTransfersPath[MAX_STATIONS];
    int transfersLength = FindMinTransfersPath(start, end, minTransfersPath);
    printpath(minTransfersPath, transfersLength);

    // ����վ����
    cout << "����վ������·��: \n";
    int ShortestPath[MAX_STATIONS];
    int ShortestLength = FindShortestPath(start, end, ShortestPath);
    printpath(ShortestPath, ShortestLength);
    return 0;
}

