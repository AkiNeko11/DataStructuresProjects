#define _CRT_SECURE_NO_WARNINGS 1

/*
2、迷宫问题（必做）（栈与队列）或者八皇后问题
[问题描述]
利用栈操作实现迷宫问题求解。
[基本要求]
（1）从文件中读取数据，生成模拟迷宫地图，不少于20行20列。
（2）给出任意入口和出口，显示输出迷宫路线。

*/

#include<iostream>
#include<stdio.h>
#include<cmath>
#define STACKINITSIZE 256
#define STACKINCREMENT 128
#define N 8
using namespace std;

struct Point
{
	int col;
	int row;
};
//Queen

struct stack
{
	Point* pbase;
	Point* ptop;
	int stacksize;
};
//stack

void InitStack(stack& s)
{
	s.pbase = (Point*)malloc(sizeof(Point) * STACKINITSIZE);
	if (s.pbase == NULL)
	{
		cout << "fail to create stack" << endl;
		exit(0);
	}
	s.ptop = s.pbase;
	s.stacksize = STACKINITSIZE;
}
//initstack

void DestroyStack(stack& s)
{
	if (s.pbase != NULL)
	{
		free(s.pbase);
		s.pbase = NULL;
		s.ptop = NULL;
		s.stacksize = 0;
	}
}
//destroy stack

void ClearStack(stack& s)
{
	s.ptop = s.pbase;
}
//clear stack

bool StackEmpty(stack s)
{
	if (s.pbase == s.ptop)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//empty 

int StackLength(stack s)
{
	return (s.ptop - s.pbase);
}
//length

void GetTop(stack s, Point& e)
{
	if (!StackEmpty(s))
	{
		e = *(s.ptop - 1);
	}
}
//get top

void StackTravese(stack s)
{
	for (int i = 0; i < StackLength(s); i++)
	{
		cout << "(" << s.pbase[i].col << "," << s.pbase[i].row << ")" << endl;
	}
	cout << endl;
}
//travese show

void Push(stack& s,Point e)
{
	if (s.ptop - s.pbase >= s.stacksize)
	{
		s.pbase = (Point*)realloc(s.pbase, (s.stacksize + STACKINCREMENT) * sizeof(Point));
		if (s.pbase == NULL)
		{
			cout << "fail to expand" << endl;
			exit(0);
		}
		s.ptop = s.pbase + s.stacksize;
		s.stacksize = s.stacksize + STACKINCREMENT;
	}
	*s.ptop = e;
	s.ptop++;
}
//push

void Pop(stack& s, Point& e)
{
	if (StackEmpty(s))
	{
		cout << "fail to pop cuz stack is empty" << endl;
		exit(0);
	}
	e = *(s.ptop - 1);
	s.ptop--;
}
//pop


int JudegeQueenConfliction(Point newQueen, stack stkQueen)
{
	int x1 = newQueen.col;
	int y1 = newQueen.row;

	int tag = 1;

	int length = StackLength(stkQueen);

	for (int i = 0; i < length; i++)
	{
		int x2 = stkQueen.pbase[i].col;
		int y2 = stkQueen.pbase[i].row;

		if (x1 == x2)
		{
			tag = 0;
			break;
		}

		if (abs(x1 - x2) == abs(y1 - y2))
		{
			tag = 0;
			break;
		}
	}

	return tag;
}
//judege

void PlaceQueen(int row, stack& stkQueen, int& resultcount)
{
	Point CurQueen;
	for (int col = 0; col < N; col++)
	{
		CurQueen.col = col;
		CurQueen.row = row;

		if (JudegeQueenConfliction(CurQueen, stkQueen))
		{
			Push(stkQueen, CurQueen);
			if (row < N - 1)
			{
				PlaceQueen(row + 1, stkQueen, resultcount);
			}
			else
			{
				StackTravese(stkQueen);
				resultcount++;
			}
			Pop(stkQueen, CurQueen);
		}
	}
}
//place

int main()
{
	stack stkQueen;
	int resultcount = 0;
	InitStack(stkQueen);
	PlaceQueen(0, stkQueen, resultcount);
	cout << "总解法数量: " << resultcount << endl;
	DestroyStack(stkQueen);
	return 0;
}