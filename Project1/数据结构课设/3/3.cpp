#define _CRT_SECURE_NO_WARNINGS 1

/*
3、家谱管理系统（必做）（树）
[问题描述]
实现具有下列功能的家谱管理系统。
[基本要求]
（1）输入文件以存放最初家谱中各成员的信息，成员的信息中均应包含以下内容：姓名、出生日期、婚否、地址、健在否、死亡日期（若其已死亡），也可附加其它信息、但不是必需的。
（2）实现数据的文件存储和读取。
（3）以图形方式显示家谱。
（4）显示第n 代所有人的信息。
（5）按照姓名查询，输出成员信息（包括其本人、父亲、孩子的信息）。
（6）按照出生日期查询成员名单。
（7）输入两人姓名，确定其关系。
（8）某成员添加孩子。
（9）删除某成员（若其还有后代，则一并删除）。
（10）修改某成员信息。
（11）要求建立至少40个成员的数据，以较为直观的方式显示结果，并提供文稿形式以便检查。
（12）界面要求：有合理的提示，每个功能可以设立菜单，根据提示，可以完成相关的功能要求。
（13）存储结构：根据系统功能要求自行设计，但是要求相关数据要存储在数据文件中。测试数据：要求使用1、全部合法数据；2、局部非法数据。进行程序测试，以保证程序的稳定。

*/

#include<stdio.h>
#include<iostream>
#include<fstream>
#define status int
#define MAX_SIZE 256

using namespace std;

typedef struct persondata
{
	//必需
	char name[20];         //姓名
	char birthdate[20];    //出生日期
	int marry;            //0  未婚   1   已婚
	char address[50];      //地址
	int death;            //0  健在   1   去世
	char deathdate[20];    //死亡日期

	//额外
	int sex;              //0-男   1-女
}persondata;

typedef struct TNode
{
	persondata person;     //成员数据
	TNode* lchild;         //左孩子
	TNode* rsibling;       //右sbling
	TNode* parent=NULL;         //父亲结点
}TNode;


status CreateTree(TNode*& T,FILE*& fp);                         //创造家谱树   先根序

status Updateparent(TNode*& T);                                 //更新父母

status ShowInPic(TNode* T);                                     //以图形方式显示

status ShowInGe(TNode* T);                                      //层次输出

status ShowGeneration(TNode* T, int n);                         //显示第n代所有人信息

TNode* NameSearch(TNode* T, char* n);                           //姓名查找

status NameSearchShow(TNode* T, char* n);                       //查找姓名输出信息

void BirthdateSearch(TNode* T, char* bd,TNode* arr[],int &n);   //根据出生日期查找成员

status BirthdateSearchShow(TNode* T, char* bd);                 //按照生日输出成员

status FindCommonAncestor(TNode* T, char* na, char* nb);        //输入两人姓名，确定关系

status AddChild(TNode* T, char* n);                             //某成员添加孩子

status DelPerson(TNode* &T, char* n);                           //删除某成员及其后代

void DelAllFollowNode(TNode* p);                                //删除某个结点的所有子树和本身

status Revise(TNode* &T, char* n);                              //修改某成员信息

status Print(TNode* person);                                    //打印输出成员信息

status FilePrint(TNode* T, FILE*& fp);                          //输出到文件里

status FilePr(TNode* T, FILE*& fp);                             //文件输出子函数

void ShowMenu();                                                //菜单函数

status Print(TNode* T)
{
	if (T == NULL)
	{
		cout << "无" << endl;
		return 0;
	}
	
	if (T->person.death == 0)
	{
		printf("姓名: %s \n出生日期: %s \n性别: %s \n婚姻状况: %s \n地址: %s \n健在\n", T->person.name, T->person.birthdate, T->person.sex==1 ? "女":"男", T->person.marry==1?"已婚":"未婚", T->person.address);
	}
	else if (T->person.death == 1)
	{
		printf("姓名: %s \n出生日期: %s \n性别: %s \n婚姻状况: %s \n地址: %s \n去世\n死亡日期: %s\n", T->person.name, T->person.birthdate, T->person.sex == 1 ? "女": "男", T->person.marry == 1 ? "已婚" : "未婚",T->person.address,T->person.deathdate);
	}
	cout << "------------------------" << endl;
	return 1;
}
//打印输出

status CreateTree(TNode*& T,FILE*& fp)
{
	if (!feof(fp))
	{
		char name1[20];         //姓名
		char birthdate1[20];    //出生日期
		int marry1;            //0  未婚   1   已婚
		char address1[50];       //地址
		int death1;            //0  健在   1   去世
		char deathdate1[20];    //死亡日期
		int sex1;              //0  male   1  female


		fscanf(fp, "%s %s %d %s %d %s %d", &name1, &birthdate1, &marry1, &address1, &death1, &deathdate1, &sex1);
		if (sex1==-1)
		{
			T = NULL;
		}
		else
		{
			T = new TNode;

			strcpy(T->person.name, name1);
			strcpy(T->person.birthdate, birthdate1);
			T->person.marry = marry1;
			strcpy(T->person.address, address1);
			T->person.death = death1;
			strcpy(T->person.deathdate, deathdate1);
			T->person.sex = sex1;

			CreateTree(T->lchild, fp);

			CreateTree(T->rsibling, fp);
			
		}

		return 1;
	}
	return -1;
}
//先根序创建 

status Updateparent(TNode*& T)
{
	if (!T)
	{
		return -1;
	}

	TNode* child = T->lchild; // 获取长子
	while (child) 
	{
		child->parent = T;

		Updateparent(child);

		child = child->rsibling;
	}
	return 1;
}
//更新孩子的父母

status ShowGeneration(TNode* T, int n)
{
	if (T == NULL)
	{
		cout << "这家全家活暗暗了" << endl;
		return 0;
	}

	TNode* p;
	TNode* qu[MAX_SIZE];
	int level[MAX_SIZE];    //记录代数

	int front, rear;
	front = rear = 0;

	qu[rear] = T;
	level[rear] = 1;
	rear++;

	int hasNodes = false;

	while (front != rear)
	{
		p = qu[front];

		int curLevel = level[front];

		front = (front + 1) % MAX_SIZE;

		if (curLevel == n)
		{
			Print(p);
			hasNodes = true;
		}

		if (curLevel > n)
		{
			break;
		}

		p = p->lchild;
		while (p)
		{
			qu[rear] = p;
			level[rear] = curLevel + 1;
			rear = (rear + 1) % MAX_SIZE;
			p = p->rsibling;
		}
	}

	if (!hasNodes)
	{
		cout << "第" << n << "代无成员" << endl;
	}
	return 1;
}
//输出第n代所有人信息

TNode* NameSearch(TNode* T, char* n)
{
	if (T == NULL)
	{
		return NULL;
	}

	if (strcmp(T->person.name, n) == 0)
	{
		return T;
	}
	
	TNode* l = NameSearch(T->lchild, n);
	TNode* r = NameSearch(T->rsibling, n);

	if (l)
	{
		return l;
	}
	if (r)
	{
		return r;
	}

	return NULL;
}
//姓名查找

status NameSearchShow(TNode* T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "错误输入" << endl;
		return 0;
	}

	cout << "本人信息:\n" << endl;
	Print(target);
	cout << "\n\n\n父母信息:\n" << endl;
	Print(target->parent);
	cout << "\n\n\n孩子信息:\n" << endl;
	TNode* p = target->lchild;
	if (!p)
	{
		Print(p);
	}
	while (p)
	{
		Print(p);
		p = p->rsibling;
	}
}
//输出个人信息

void BirthdateSearch(TNode* T, char* bd,TNode* arr[],int &n)
{
	if (T == NULL)
	{
		return ;
	}

	if (strcmp(T->person.birthdate, bd) == 0)
	{
		arr[n] = T;
		n++;
	}

	BirthdateSearch(T->lchild, bd, arr, n);
	BirthdateSearch(T->rsibling, bd, arr, n);
}
//生日查找

status BirthdateSearchShow(TNode* T, char* bd)
{
	TNode* arr[MAX_SIZE] = { NULL };
	int n = 0;
	BirthdateSearch(T, bd, arr, n);
	cout << "共有" << n << "位成员" << endl;
	for (int i = 0; i < n; i++)
	{
		Print(arr[i]);
	}
	return 1;
}
//生日输出

status FindCommonAncestor(TNode* T, char* na, char* nb)
{
	TNode* a = NameSearch(T, na);
	TNode* b = NameSearch(T, nb);

	if (!a || !b)
	{
		cout << "错误输入" << endl;
		return -1;
	}

	for (TNode* pa = a; pa != NULL; pa = pa->parent)
	{
		for (TNode* pb = b; pb != NULL; pb = pb->parent)
		{
			if (pa == pb)
			{
				cout << "两人共同祖先:" << endl;
				Print(pa);
				return 1;
			}
		}
	}
}
//查找两人关系（共同祖先）

status AddChild(TNode* T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "输入错误" << endl;
		return -1;
	}
	cout << "请输入新孩子信息" << endl;
	TNode* newchild = new TNode;
	
	char name1[20];         //姓名
	char birthdate1[20];    //出生日期
	int marry1;            //0  未婚  , 1   已婚
	char address1[50];      //地址
	int death1;            //0  健在  , 1   去世
	char deathdate1[20];    //死亡日期

	int sex1;              //0  男  ,   1  女

	cout << "请输入姓名" << endl;
	scanf("%s", &name1);

	cout << "请输入出生日期" << endl;
	scanf("%s", &birthdate1);

	cout << "请输入性别(0  男 , 1  女)" << endl;
	scanf("%d", &sex1);

	cout << "请输入婚姻状况(0  未婚 , 1  已婚)" << endl;
	scanf("%d", &marry1);

	cout << "请输入地址" << endl;
	scanf("%s", &address1);

	cout << "请输入死亡情况(0  健在 , 1  去世)" << endl;
	scanf("%d", &death1);

	if (death1 == 1)
	{
		cout << "请输入死亡日期" << endl;
		scanf("%s", &deathdate1);
	}
	else
	{
		{
			deathdate1[0] = 'N';
		}
	}

	strcpy(newchild->person.name, name1);
	strcpy(newchild->person.birthdate, birthdate1);
	newchild->person.marry = marry1;
	strcpy(newchild->person.address, address1);
	newchild->person.death = death1;
	strcpy(newchild->person.deathdate, deathdate1);
	newchild->person.sex = sex1;


	newchild->parent = target;
	newchild->lchild = NULL;
	newchild->rsibling = NULL;

	if (target->lchild == NULL)
	{
		target->lchild = newchild;
		return 1;
	}

	TNode* p = target->lchild;
	target->lchild = newchild;
	newchild->rsibling = p;
	return 1;
}
//增加孩子

status DelPerson(TNode*& T, char* n)
{
	TNode* target = NameSearch(T, n);

	if (!target)
	{
		cout << "错误输入" << endl;
		return -1;
	}

	if (target == T)
	{
		DelAllFollowNode(T);
		T = NULL;
		return 1;
	}

	if (target->lchild)
	{
		TNode* p = target->lchild;
		DelAllFollowNode(p);
		target->lchild = NULL;
	}

	TNode* tp = target->parent;

	if (tp->lchild == target)
	{
		tp->lchild = target->rsibling;
		delete target;
		return 1;
	}
	else
	{
		TNode* temp = tp->lchild;
		while (temp->rsibling != target)
		{
			temp = temp->rsibling;
		}
		temp->rsibling = temp->rsibling->rsibling;
		delete target;
		return 1;
	}
}
//删除成员

void DelAllFollowNode(TNode* p)
{
	if (p == NULL)
	{
		return ;
	}

	DelAllFollowNode(p->lchild);
	DelAllFollowNode(p->rsibling);
	delete p;
}
//删除结点的全部子树和本身

status Revise(TNode* &T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "错误输入" << endl;
		return -1;
	}

	cout << "请输入新信息" << endl;

	char name1[20];         //姓名
	char birthdate1[20];    //出生日期
	int marry1;            //0  未婚  , 1   已婚
	char address1[50];      //地址
	int death1;            //0  健在  , 1   去世
	char deathdate1[20];    //死亡日期

	int sex1;              //0  男  ,   1  女

	cout << "请输入姓名" << endl;
	scanf("%s", &name1);

	cout << "请输入出生日期" << endl;
	scanf("%s", &birthdate1);

	cout << "请输入性别(0  男 , 1  女)" << endl;
	scanf("%d", &sex1);

	cout << "请输入婚姻状况(0  未婚 , 1  已婚)" << endl;
	scanf("%d", &marry1);

	cout << "请输入地址" << endl;
	scanf("%s", &address1);

	cout << "请输入死亡情况(0  健在 , 1  去世)" << endl;
	scanf("%d", &death1);

	if (death1 == 1)
	{
		cout << "请输入死亡日期" << endl;
		scanf("%s", &deathdate1);
	}
	else
	{
		{
			deathdate1[0] = 'N';
		}
	}

	strcpy(target->person.name, name1);
	strcpy(target->person.birthdate, birthdate1);
	target->person.marry = marry1;
	strcpy(target->person.address, address1);
	target->person.death = death1;
	strcpy(target->person.deathdate, deathdate1);
	target->person.sex = sex1;

	cout << "修改完成！" << endl;
	return 1;
}
//修改信息

status ShowInPic(TNode* T)
{
	if (T != NULL)
	{
		cout << T->person.name;
		if (T->lchild)
		{
			cout << "(";
			TNode* p = T->lchild;
			ShowInPic(p);
			p = p->rsibling;
			while (p)
			{
				cout << ",";
				ShowInPic(p);
				p = p->rsibling;
			}
			cout << ")";
		}
		return 1;
	}
	cout << "这家全家活暗暗了" << endl;
	return 0;
}
//以图形显示

status ShowInGe(TNode* T)
{
	if (T == NULL)
	{
		return -1;
	}

	TNode* p;
	TNode* qu[MAX_SIZE];  // 队列，用于存储节点
	int level[MAX_SIZE];  // 队列中对应的层数
	int front = 0, rear = 0;

	qu[rear] = T;         // 根节点入队
	level[rear] = 1;      // 根节点层数为1
	rear++;

	int currentLevel = 1; // 当前输出的层数

	while (front != rear)
	{
		p = qu[front];          // 取出队头节点
		int nodeLevel = level[front]; // 当前节点所在层数
		front = (front + 1) % MAX_SIZE;

		if (nodeLevel > currentLevel) // 如果是新的一层
		{
			cout << endl;       // 换行
			currentLevel = nodeLevel;
		}

		printf("%5s (父: %5s)", p->person.name, (p->parent ? p->parent->person.name : "NULL"));

		//cout << p->person.name << "(父母:" << (p->parent ? p->parent->person.name : "NULL") << ")     "; // 输出节点数据

		p = p->lchild;      // 子节点入队
		while (p)
		{
			qu[rear] = p;
			level[rear] = nodeLevel + 1; // 子节点层数为当前层数+1
			rear++;
			p = p->rsibling;
		}
	}

	cout << endl; // 最后换行
	return 1;
}
//层次输出

void ShowMenu() 
{
	cout << "\n===== 家谱管理系统 =====" << endl;
	cout << "1. 显示家谱" << endl;
	cout << "2. 显示第N代人所有人信息" << endl;
	cout << "3. 按照姓名查询，输出成员信息（包括其本人、父亲、孩子的信息）" << endl;
	cout << "4. 按照出生日期查询成员名单" << endl;
	cout << "5. 输入两人姓名，确定其关系" << endl;
	cout << "6. 某成员新生孩子" << endl;
	cout << "7. 把某成员逐出家门" << endl;
	cout << "8. 修改某成员信息" << endl;
	cout << "9. 退出程序" << endl;
	cout << "0. 输出家谱到文件" << endl;
	cout << "请选择功能（0-9）：";
}
//菜单函数

status FilePrint(TNode* T, FILE*& fp2)
{
	fp2 = fopen("familyout.txt", "w");
	if (!fp2)
	{
		cout << "打开2文件失败！" << endl;
		return -1;
	}
	FilePr(T, fp2);

	fclose(fp2);
	cout << "成功输出！" << endl;
	return 1;

}
//输出到文件里

status FilePr(TNode* T, FILE*& fp2)
{
	if (T == NULL)
	{
		fprintf(fp2, "NULL\n");
		fprintf(fp2, "----------------------------\n");
		return 0;
	}
	
	fprintf(fp2, "name: %s \nbirthdate: %s \nmarry: %d \naddress: %s \nDeadorAlive: %d\ndeathdate: %8s\nsex: %d \n", T->person.name, T->person.birthdate, T->person.marry , T->person.address, T->person.death,T->person.deathdate ,T->person.sex);
	
	fprintf(fp2, "----------------------------\n");
	
	FilePr(T->lchild, fp2);
	FilePr(T->rsibling, fp2);
	return 1;
}
//文件输出子函数


int main() 
{
	char choice = 0;

	TNode* T=NULL;

	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	
	fp1 = fopen("family.txt", "r");

	if (!fp1)
	{
		cout <<"打开文件失败！"<< endl;
		return -1;
	}

	CreateTree(T, fp1);

	Updateparent(T);

	fclose(fp1);

	while (true) 
	{
		cout << endl;
		ShowMenu();
		cin >> choice;

		int n;
		char s[20];
		char t[20];

		// 根据用户选择调用对应功能
		switch (choice) 
		{
		case '1':
			cout << endl;
			ShowInPic(T);
			cout << endl;
			ShowInGe(T);
			cout << "\n显示完成!" << endl;
			break;
		case '2':
			cout << endl;
			cout << "请输入查询代数" << endl;
			cin >> n;
			cout << endl;
			ShowGeneration(T, n);
			cout << "\n显示完成!" << endl;
			break;
		case '3':
			cout << endl;
			cout << "请输入姓名" << endl;
			cin >> s;
			cout << endl;
			NameSearchShow(T, s);
			cout << "\n显示完成!" << endl;
			break;
		case '4':
			cout << endl;
			cout << "请输入生日" << endl;
			cin >> s;
			cout << endl;
			BirthdateSearchShow(T, s);
			cout << "\n显示完成!" << endl;
			break;
		case '5':
			cout << endl;
			cout << "请输入两人姓名" << endl;
			cin >> s;
			cin >> t;
			cout << endl;
			FindCommonAncestor(T, s, t);
			cout << "\n显示完成!" << endl;
			break;
		case '6':
			cout << endl;
			cout << "请输入父亲姓名" << endl;
			cin >> s;
			cout << endl;
			AddChild(T, s);
			cout << "\n添加完成!" << endl;
			break;
		case '7':
			cout << endl;
			cout << "请输入被逐出者姓名" << endl;
			cin >> s;
			cout << endl;
			DelPerson(T, s);
			cout << "\n删除完成!" << endl;
			break;
		case '8':
			cout << endl;
			cout << "请输入姓名" << endl;
			cin >> s;
			cout << endl;
			Revise(T, s);
			cout << "\n修改完成!" << endl;
			break;
		case '9':
			return 0; // 退出程序
		case '0':
			FilePrint(T, fp2);
			break;
		default:
			cout << endl;
			cout << "无效选择，请重新输入！" << endl;
			break;
		}
	}
	return 0;
}
// 主函数