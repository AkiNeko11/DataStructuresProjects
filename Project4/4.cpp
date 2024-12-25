#define _CRT_SECURE_NO_WARNINGS 1
#include <windows.h> //          Sleep
#include<stdlib.h>   //          system("cls")
#include <tlhelp32.h>//          ���̿��պ���ͷ�ļ�
#include <stdio.h>
#include <iostream>
#include <psapi.h>
#include <cwchar>    //          wchar--char*
#include <conio.h>   //          ���� _kbhit() �� _getch()ʵ����ͣ
#define INTERVAL 1000//          ÿ1��ͳ��һ��
using namespace std;

typedef int Status;

typedef struct DLNode
{
	int ID;            //����ID
	int Memory;        //����ʹ���ڴ�
	char Name[200];    //������
	int Duration;      //���̳���ʱ��

	DLNode* next;
}DLNode, * DLinkList;
//��ǰ�����

typedef struct SLNode
{
	int ID;            //����ID
	int Memory;        //����ʹ���ڴ�
	char Name[200];    //������
	int Duration;      //���̳���ʱ��

	int Endtime;       //���̽���ʱ��

	SLNode* pre;
	SLNode* next;
}SLNode,*SLinkList;
//��������

Status SortDList(DLinkList& L);         //����ʹ���ڴ��Զൽ�ٸ���ǰ��������
Status SortSList(SLinkList& S);         //���ճ���ʱ�����ٵ����������������
Status CreateList(DLinkList& L);        //������ǰ��������
Status CreateOverList(SLinkList& S, DLinkList L, DLinkList currentL );    //�����ѽ�����������

Status UpdateList(DLinkList& L, DLinkList currentL);         //���µ�ǰ����
Status FreeDList(DLinkList& currentL);                       //�ͷ�cL

void ShowD(DLinkList L);          //��ʾ��ǰ�������
void ShowS(SLinkList S);          //��ʾ�����������

int main()
{
	SLinkList S = NULL;                       //�ѽ�����������S
	DLinkList L = NULL;                       //�ܽ��̱���һ�εĵ�ǰ���̱����뵱ǰ���̱�ȶ�
	DLinkList currentL = NULL;                //��ǰ���̱�

	CreateList(L);                     //����������ܽ��̱�
	currentL = L;                      //��ʼcL��ΪL

	bool isRunning = true;             // �����Ƿ���ͣ����
	char key;


	while (1)
	{

		if (_kbhit()) 
		{                               // ����Ƿ��а�������
			key = _getch();             // ��ȡ���µļ�
			if (key == 'p') 
			{
				isRunning = false;      // ��ͣ����
				cout << "\n��������ͣ������ 'c' ������" << endl;
			}
			else if (key == 'c') 
			{
				isRunning = true;       // ��������
				cout << "\n����������С�" << endl;
			}
			else if (key == 'q') 
			{
				cout << "\n�����˳���" << endl;
				break;                  // �˳�����
			}
		}

		if (isRunning)
		{
			Sleep(INTERVAL);   //ÿ  INTERVAL  msˢ��һ��
			system("cls");     //����

			CreateList(currentL);                   //������ǰ����

			CreateOverList(S, L, currentL);         //������һ�εĵ�ǰ���̺͵�ǰ���̴���S    
			SortSList(S);                           //����S

			UpdateList(L, currentL);                //���µ�ǰ����
			SortDList(L);                           //����L

			FreeDList(currentL);                    //�ͷ�cL

			cout << "��ǰ����:" << endl;
			ShowD(L);
			cout << endl;
			cout << "��������:" << endl;
			ShowS(S);
			cout << "\n����p��ͣ,c����,q�˳�" << endl;
		}
	}
}


Status CreateList(DLinkList& L)
{
	L = (DLinkList)malloc(sizeof(DLNode));
	DLinkList p = L;//ͷ�ڵ�
	DLinkList q = (DLinkList)malloc(sizeof(DLNode));

	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;

	int countProcess = 0;//��ǰ��������
	int countNoPower = 0;//û��Ȩ�޷��ʵĽ�������

	PROCESSENTRY32 currentProcess;//��Ž�����Ϣ
	currentProcess.dwSize = sizeof(currentProcess);
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ȡϵͳ�ڵ����н���

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot()��������ʧ��!\n");
		return -1;
	}

	bool bMore = Process32First(hProcess, &currentProcess); //��ȡ��һ��������Ϣ
	while (bMore)
	{
		int judge = 1;//�ж��ܷ����

		if (!(handle = OpenProcess(PROCESS_ALL_ACCESS, 0, currentProcess.th32ProcessID)))//��handleΪ���ΪID���̵�Ȩ��
		{
			countNoPower++;
			judge = 0;
		}//�кܶ����openprocess������Ȩ�޷��ʣ���������Ȩ

		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));//��������ĳ�����еõ��ڴ��С

		if (judge)
		{
			q = (DLinkList)malloc(sizeof(DLNode));
			if (!q)exit(-2);

			q->Memory = pmc.WorkingSetSize / 1024;
			q->ID = currentProcess.th32ProcessID;
			q->Duration = 0;//ʱ���ʼΪ0

			char temp[200];
			wcstombs(temp, currentProcess.szExeFile, sizeof(temp));
			strcpy(q->Name, temp);                       //wchar-----char*
			p->next = q;
			p = q;
		}
		bMore = Process32Next(hProcess, &currentProcess);                                                //������һ��
		countProcess++;
	}
	CloseHandle(hProcess); //���hProcess���
	L->ID = countProcess - countNoPower;//ͷ�ڵ㴢����Ȩ�޷��ʵĳ�����
	L->Memory = 0;
	const char* na = "Phead";
	strcpy(L->Name, na);

	L->Duration = 0;
	q->next = NULL;
	return 1;
}

Status CreateOverList(SLinkList& S, DLinkList L, DLinkList currentL)
{
	//�����������̱�,�ȱ���cL��S�Ƚϣ��ж��Ƿ������������ٱ���L��cL�Ƚϣ��ж��Ƿ����½�������,Sduration=Ld

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
					if (strcmp(cLq->Name, Sq->Name) == 0)//�����ĳ���                             12.24:  ID����Name
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

						Sp->ID--;//����������Ŀ-1
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
				int find = 0;//��ǽ����Ƿ�������

				while (cLq)
				{
					if (cLq->ID == Lq->next->ID)//�ҵ�����                            12.24:ID����Name           
					{
						find = 1;
						break;
					}
					cLq = cLq->next;
				}


				if (find == 0)//���µĽ�����û�ҵ���˵���ý��̽���,���
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
				int find = 0;//��ǽ����Ƿ�������

				while (cLq)
				{
					if (cLq->ID == Lq->next->ID)//�ҵ�����
					{
						find = 1;
						Lq->next->Memory = cLq->Memory;                   //md����Lq->next��д��Lq����һ������ŷ��֣���������12.24
						Lq->next->Duration+=INTERVAL/1000;
						break;
					}
					cLq = cLq->next;
				}


				if (find == 0)//���µĽ�����û�ҵ���˵���ý��̽���,ɾ��
				{
					DLinkList temp = Lq->next;
					Lq->next = Lq->next->next;
					free(temp);
					Lp->ID--;//���н�����Ŀ-1
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
					if (strcmp(Lq->Name, cLq->Name) == 0)                                                 //12.24:ID����Name
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
			printf(" ������: %40s ����ID: %7d  ����ʱ��:%3d  ����ʱ��:%3d \n", p->Name, p->ID, p->Duration, p->Endtime);
			p = p->next;
		}
	}
	cout << "����������:" << S->ID << endl;
}

void ShowD(DLinkList L)
{
	int i = 0;
	DLinkList  p = L->next;
	while (p)
	{
		printf(" �ڴ�ʹ��%7dKB  ������: %40s ����ID: %7d  ����ʱ��:%3d \n", p->Memory, p->Name,p->ID, p->Duration);
		p = p->next;
	}
	cout <<"Ŀǰ������:" << L->ID << endl;
}

Status SortDList(DLinkList& L)
{
	if (L)
	{
		int i = 1, j = 1;
		DLinkList q, t = (DLinkList)malloc(sizeof(DLNode));
		for (i = 1; i <= L->ID; i++)//��������
		{
			for (q = L->next, j = 1; q->next != NULL && j <= L->ID - i; q = q->next, j++)
			{
				if (q->Memory < q->next->Memory)//�����ڴ�Ӵ�С����
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

Status SortSList(SLinkList& S)//���ս���ʱ������
{
	if (S)
	{
		int i = 1, j = 1;
		SLinkList q, t = (SLinkList)malloc(sizeof(SLNode));
		for (i = 1; i <= S->ID; i++)//��������
		{
			for (q = S->next, j = 1; q->next != NULL && j <= S->ID - i; q = q->next, j++)
			{
				if (q->Duration > q->next->Duration)//���ݳ���ʱ���С��������
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