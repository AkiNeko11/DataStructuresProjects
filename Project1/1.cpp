#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h> //          Sleep
#include<stdlib.h>   //          system("cls")
#include <tlhelp32.h>//          进程快照函数头文件
#include <stdio.h>
#include <iostream>
#include <psapi.h>
#include <cwchar>    //          wchar--char*
#include <conio.h>   //          用于 _kbhit() 和 _getch()实现暂停
#define INTERVAL 1000//          每1秒统计一次
using namespace std;

typedef int Status;

typedef struct DLNode
{
	int ID;            //进程ID
	int Memory;        //进程使用内存
	char Name[200];    //进程名
	int Duration;      //进程持续时间

	DLNode* next;
}DLNode, * DLinkList;
//当前活动进程

typedef struct SLNode
{
	int ID;            //进程ID
	int Memory;        //进程使用内存
	char Name[200];    //进程名
	int Duration;      //进程持续时间

	int Endtime;       //进程结束时间

	SLNode* pre;
	SLNode* next;
}SLNode,*SLinkList;
//结束进程

Status SortDList(DLinkList& L);         //按照使用内存自多到少给当前进程排序
Status SortSList(SLinkList& S);         //按照持续时间自少到多给结束进程排序
Status CreateList(DLinkList& L);        //创建当前进程链表
Status CreateOverList(SLinkList& S, DLinkList L, DLinkList currentL );    //创建已结束进程链表

Status UpdateList(DLinkList& L, DLinkList currentL);         //更新当前进程
Status FreeDList(DLinkList& currentL);                       //释放cL

void ShowD(DLinkList L);          //显示当前进程情况
void ShowS(SLinkList S);          //显示结束进程情况

int main()
{
	SLinkList S = NULL;                       //已结束进程链表S
	DLinkList L = NULL;                       //总进程表（上一次的当前进程表），与当前进程表比对
	DLinkList currentL = NULL;                //当前进程表

	CreateList(L);                     //创建最初的总进程表
	currentL = L;                      //初始cL即为L

	bool isRunning = true;             // 控制是否暂停程序
	char key;


	while (1)
	{

		if (_kbhit()) 
		{                               // 检测是否有按键按下
			key = _getch();             // 获取按下的键
			if (key == 'p') 
			{
				isRunning = false;      // 暂停程序
				cout << "\n程序已暂停，按下 'c' 继续。" << endl;
			}
			else if (key == 'c') 
			{
				isRunning = true;       // 继续程序
				cout << "\n程序继续运行。" << endl;
			}
			else if (key == 'q') 
			{
				cout << "\n程序退出。" << endl;
				break;                  // 退出程序
			}
		}

		if (isRunning)
		{
			Sleep(INTERVAL);   //每  INTERVAL  ms刷新一次
			system("cls");     //清屏

			CreateList(currentL);                   //创建当前进程

			CreateOverList(S, L, currentL);         //根据上一次的当前进程和当前进程创建S    
			SortSList(S);                           //排序S

			UpdateList(L, currentL);                //更新当前进程
			SortDList(L);                           //排序L

			FreeDList(currentL);                    //释放cL

			cout << "当前进程:" << endl;
			ShowD(L);
			cout << endl;
			cout << "结束进程:" << endl;
			ShowS(S);
			cout << "\n按下p暂停,c继续,q退出" << endl;
		}
	}
}


Status CreateList(DLinkList& L)
{
	L = (DLinkList)malloc(sizeof(DLNode));
	DLinkList p = L;//头节点
	DLinkList q = (DLinkList)malloc(sizeof(DLNode));

	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;

	int countProcess = 0;//当前进程数量
	int countNoPower = 0;//没有权限访问的进程数量

	PROCESSENTRY32 currentProcess;//存放进程信息
	currentProcess.dwSize = sizeof(currentProcess);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获取系统内的所有进程

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot()函数调用失败!\n");
		return -1;
	}

	bool bMore = Process32First(hProcess, &currentProcess); //获取第一个进程信息
	while (bMore)
	{
		int judge = 1;//判断能否访问

		if (!(handle = OpenProcess(PROCESS_ALL_ACCESS, 0, currentProcess.th32ProcessID)))//令handle为编号为ID进程的权柄
		{
			countNoPower++;
			judge = 0;
		}//有很多进程openprocess函数无权限访问，需自行提权

		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));//函数，从某进程中得到内存大小

		if (judge)
		{
			q = (DLinkList)malloc(sizeof(DLNode));
			if (!q)exit(-2);

			q->Memory = pmc.WorkingSetSize / 1024;
			q->ID = currentProcess.th32ProcessID;
			q->Duration = 0;//时间初始为0

			char temp[200];
			wcstombs(temp, currentProcess.szExeFile, sizeof(temp));
			strcpy(q->Name, temp);                       //wchar-----char*
			p->next = q;
			p = q;
		}
		bMore = Process32Next(hProcess, &currentProcess);                                                //遍历下一个
		countProcess++;
	}
	CloseHandle(hProcess); //清除hProcess句柄
	L->ID = countProcess - countNoPower;//头节点储存有权限访问的程序数
	L->Memory = 0;
	const char* na = "Phead";
	strcpy(L->Name, na);

	L->Duration = 0;
	q->next = NULL;
	return 1;
}

Status CreateOverList(SLinkList& S, DLinkList L, DLinkList currentL)
{
	//创建结束进程表,先遍历cL和S比较，判断是否有重启动，再遍历L和cL比较，判断是否有新结束进程,Sduration=Ld

	if (currentL)
	{
		DLinkList cLp = currentL;
		SLinkList Sp = S;
		DLinkList Lp = L;

		if (Sp)
		{
			SLinkList Sq = Sp->next;
			while (Sq)
			{
				DLinkList cLq = cLp->next;
				while (cLq)
				{
					if (strcmp(cLq->Name, Sq->Name) == 0)//重启的程序                             12.24:  ID——Name
					{
						SLinkList temp = Sq;
						Sq = Sq->pre;
						temp->pre->next = temp->next;

						if (temp->next)
						{
							temp->next->pre = temp->pre;
							free(temp);
						}
						else
						{
							free(temp);
						}

						Sp->ID--;//结束进程数目-1
						break;
					}
					cLq = cLq->next;
				}
				Sq = Sq->next;
			}
		}

		if (!Sp)
		{
			S = (SLinkList)malloc(sizeof(SLinkList));

			S->Duration = L->Duration;
			S->Endtime = 0;
			S->ID = 0;
			S->Memory = 0;

			S->pre = NULL;
			S->next = NULL;
		}

		if (cLp)
		{
			DLinkList Lq = Lp;
			while (Lq->next)
			{
				DLinkList cLq = cLp->next;
				int find = 0;//标记进程是否还在运行

				while (cLq)
				{
					if (cLq->ID == Lq->next->ID)//找到进程                            12.24:ID——Name           
					{
						find = 1;
						break;
					}
					cLq = cLq->next;
				}


				if (find == 0)//最新的进程里没找到，说明该进程结束,添加
				{
					SLNode* newNode = (SLNode*)malloc(sizeof(SLNode));
					newNode->ID = Lq->next->ID;
					newNode->Duration = Lq->next->Duration;
					newNode->Endtime = Lp->Duration;
					strcpy(newNode->Name, Lq->next->Name);

					if (S->next)
					{
						S->next->pre = newNode;
						newNode->next = S->next;
						newNode->pre = S;
						S->next = newNode;
					}
					else
					{
						S->next = newNode;
						newNode->pre = S;
						newNode->next = NULL;
					}
					S->ID++;
				}

				Lq = Lq->next;

			}
		}
		return 1;
	}
	return -1;
}

Status UpdateList(DLinkList& L, DLinkList currentL)
{
	if (currentL)
	{
		DLinkList cLp = currentL;
		DLinkList Lp = L;

		if (cLp)
		{
			DLinkList Lq = Lp;
			while (Lq->next)
			{
				DLinkList cLq = cLp->next;
				int find = 0;//标记进程是否还在运行

				while (cLq)
				{
					if (cLq->ID == Lq->next->ID)//找到进程
					{
						find = 1;
						Lq->next->Memory = cLq->Memory;                   //md这行Lq->next误写成Lq改了一个中午才发现！！！——12.24
						Lq->next->Duration+=INTERVAL/1000;
						break;
					}
					cLq = cLq->next;
				}


				if (find == 0)//最新的进程里没找到，说明该进程结束,删除
				{
					DLinkList temp = Lq->next;
					Lq->next = Lq->next->next;
					free(temp);
					Lp->ID--;//进行进程数目-1
				}
				else
				{
					Lq = Lq->next;
				}
			}
		}

		if (Lp)
		{
			DLinkList cLq = cLp->next;
			while (cLq)
			{
				DLinkList Lq = Lp->next;
				int find = 0;
				while (Lq)
				{
					if (strcmp(Lq->Name, cLq->Name) == 0)                                                 //12.24:ID——Name
					{
						find = 1;
						break;
					}
					Lq = Lq->next;
				}

				if (find == 0)
				{
					DLinkList newNode = (DLinkList)malloc(sizeof(DLNode));
					newNode->ID = cLq->ID;
					strcpy(newNode->Name, cLq->Name);
					newNode->Memory = cLq->Memory;
					newNode->Duration = 0;

					newNode->next = Lp->next;
					Lp->next = newNode;
					Lp->ID++;
				}

				cLq = cLq->next;
			}
		}

		Lp->Duration+=INTERVAL/1000;
		return 1;
	}
	return -1;
}

void ShowS(SLinkList S)
{
	if (S->ID)
	{
		SLinkList p = S->next;
		while (p != NULL)
		{
			printf(" 进程名: %40s 进程ID: %7d  持续时间:%3d  结束时间:%3d \n", p->Name, p->ID, p->Duration, p->Endtime);
			p = p->next;
		}
	}
	cout << "结束进程数:" << S->ID << endl;
}

void ShowD(DLinkList L)
{
	int i = 0;
	DLinkList  p = L->next;
	while (p)
	{
		printf(" 内存使用%7dKB  进程名: %40s 进程ID: %7d  持续时间:%3d \n", p->Memory, p->Name,p->ID, p->Duration);
		p = p->next;
	}
	cout <<"目前进程数:" << L->ID << endl;
}

Status SortDList(DLinkList& L)
{
	if (L)
	{
		int i = 1, j = 1;
		DLinkList q, t = (DLinkList)malloc(sizeof(DLNode));
		for (i = 1; i <= L->ID; i++)//起泡排序
		{
			for (q = L->next, j = 1; q->next != NULL && j <= L->ID - i; q = q->next, j++)
			{
				if (q->Memory < q->next->Memory)//根据内存从大到小排序
				{
					t->ID = q->ID;
					t->Memory = q->Memory;
					t->Duration = q->Duration;
					strcpy(t->Name, q->Name);

					q->ID = q->next->ID;
					q->Memory = q->next->Memory;
					q->Duration = q->next->Duration;
					strcpy(q->Name, q->next->Name);

					q->next->ID = t->ID;
					q->next->Memory = t->Memory;
					q->next->Duration = t->Duration;
					strcpy(q->next->Name, t->Name);
				}
			}
		}
		return 1;
	}
	return -1;
}

Status SortSList(SLinkList& S)//按照结束时间排序
{
	if (S)
	{
		int i = 1, j = 1;
		SLinkList q, t = (SLinkList)malloc(sizeof(SLNode));
		for (i = 1; i <= S->ID; i++)//起泡排序
		{
			for (q = S->next, j = 1; q->next != NULL && j <= S->ID - i; q = q->next, j++)
			{
				if (q->Duration > q->next->Duration)//根据持续时间从小到大排序
				{
					t->Duration = q->Duration;
					t->Endtime = q->Endtime;
					t->ID = q->ID;
					strcpy(t->Name, q->Name);

					q->Duration = q->next->Duration;
					q->Endtime = q->next->Endtime;
					q->ID = q->next->ID;
					strcpy(q->Name, q->next->Name);

					q->next->Duration = t->Duration;
					q->next->Endtime = t->Endtime;
					q->next->ID = t->ID;
					strcpy(q->next->Name, t->Name);

				}
			}
		}
		return 1;
	}
	return -1;
}

Status FreeDList(DLinkList& currentL)
{
	DLNode* Lp = currentL;
	while (Lp)
	{
		DLNode* t = Lp;
		Lp = Lp->next;
		free(t);
	}
	return 1;
}