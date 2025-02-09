#define _CRT_SECURE_NO_WARNINGS 1

/*
3�����׹���ϵͳ��������������
[��������]
ʵ�־������й��ܵļ��׹���ϵͳ��
[����Ҫ��]
��1�������ļ��Դ����������и���Ա����Ϣ����Ա����Ϣ�о�Ӧ�����������ݣ��������������ڡ���񡢵�ַ�����ڷ��������ڣ���������������Ҳ�ɸ���������Ϣ�������Ǳ���ġ�
��2��ʵ�����ݵ��ļ��洢�Ͷ�ȡ��
��3����ͼ�η�ʽ��ʾ���ס�
��4����ʾ��n �������˵���Ϣ��
��5������������ѯ�������Ա��Ϣ�������䱾�ˡ����ס����ӵ���Ϣ����
��6�����ճ������ڲ�ѯ��Ա������
��7����������������ȷ�����ϵ��
��8��ĳ��Ա��Ӻ��ӡ�
��9��ɾ��ĳ��Ա�����仹�к������һ��ɾ������
��10���޸�ĳ��Ա��Ϣ��
��11��Ҫ��������40����Ա�����ݣ��Խ�Ϊֱ�۵ķ�ʽ��ʾ��������ṩ�ĸ���ʽ�Ա��顣
��12������Ҫ���к������ʾ��ÿ�����ܿ��������˵���������ʾ�����������صĹ���Ҫ��
��13���洢�ṹ������ϵͳ����Ҫ��������ƣ�����Ҫ���������Ҫ�洢�������ļ��С��������ݣ�Ҫ��ʹ��1��ȫ���Ϸ����ݣ�2���ֲ��Ƿ����ݡ����г�����ԣ��Ա�֤������ȶ���

*/

#include<stdio.h>
#include<iostream>
#include<fstream>
#define status int
#define MAX_SIZE 256

using namespace std;

typedef struct persondata
{
	//����
	char name[20];         //����
	char birthdate[20];    //��������
	int marry;            //0  δ��   1   �ѻ�
	char address[50];      //��ַ
	int death;            //0  ����   1   ȥ��
	char deathdate[20];    //��������

	//����
	int sex;              //0-��   1-Ů
}persondata;

typedef struct TNode
{
	persondata person;     //��Ա����
	TNode* lchild;         //����
	TNode* rsibling;       //��sbling
	TNode* parent=NULL;         //���׽��
}TNode;


status CreateTree(TNode*& T,FILE*& fp);                         //���������   �ȸ���

status Updateparent(TNode*& T);                                 //���¸�ĸ

status ShowInPic(TNode* T);                                     //��ͼ�η�ʽ��ʾ

status ShowInGe(TNode* T);                                      //������

status ShowGeneration(TNode* T, int n);                         //��ʾ��n����������Ϣ

TNode* NameSearch(TNode* T, char* n);                           //��������

status NameSearchShow(TNode* T, char* n);                       //�������������Ϣ

void BirthdateSearch(TNode* T, char* bd,TNode* arr[],int &n);   //���ݳ������ڲ��ҳ�Ա

status BirthdateSearchShow(TNode* T, char* bd);                 //�������������Ա

status FindCommonAncestor(TNode* T, char* na, char* nb);        //��������������ȷ����ϵ

status AddChild(TNode* T, char* n);                             //ĳ��Ա��Ӻ���

status DelPerson(TNode* &T, char* n);                           //ɾ��ĳ��Ա������

void DelAllFollowNode(TNode* p);                                //ɾ��ĳ���������������ͱ���

status Revise(TNode* &T, char* n);                              //�޸�ĳ��Ա��Ϣ

status Print(TNode* person);                                    //��ӡ�����Ա��Ϣ

status FilePrint(TNode* T, FILE*& fp);                          //������ļ���

status FilePr(TNode* T, FILE*& fp);                             //�ļ�����Ӻ���

void ShowMenu();                                                //�˵�����

status Print(TNode* T)
{
	if (T == NULL)
	{
		cout << "��" << endl;
		return 0;
	}
	
	if (T->person.death == 0)
	{
		printf("����: %s \n��������: %s \n�Ա�: %s \n����״��: %s \n��ַ: %s \n����\n", T->person.name, T->person.birthdate, T->person.sex==1 ? "Ů":"��", T->person.marry==1?"�ѻ�":"δ��", T->person.address);
	}
	else if (T->person.death == 1)
	{
		printf("����: %s \n��������: %s \n�Ա�: %s \n����״��: %s \n��ַ: %s \nȥ��\n��������: %s\n", T->person.name, T->person.birthdate, T->person.sex == 1 ? "Ů": "��", T->person.marry == 1 ? "�ѻ�" : "δ��",T->person.address,T->person.deathdate);
	}
	cout << "------------------------" << endl;
	return 1;
}
//��ӡ���

status CreateTree(TNode*& T,FILE*& fp)
{
	if (!feof(fp))
	{
		char name1[20];         //����
		char birthdate1[20];    //��������
		int marry1;            //0  δ��   1   �ѻ�
		char address1[50];       //��ַ
		int death1;            //0  ����   1   ȥ��
		char deathdate1[20];    //��������
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
//�ȸ��򴴽� 

status Updateparent(TNode*& T)
{
	if (!T)
	{
		return -1;
	}

	TNode* child = T->lchild; // ��ȡ����
	while (child) 
	{
		child->parent = T;

		Updateparent(child);

		child = child->rsibling;
	}
	return 1;
}
//���º��ӵĸ�ĸ

status ShowGeneration(TNode* T, int n)
{
	if (T == NULL)
	{
		cout << "���ȫ�һ����" << endl;
		return 0;
	}

	TNode* p;
	TNode* qu[MAX_SIZE];
	int level[MAX_SIZE];    //��¼����

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
		cout << "��" << n << "���޳�Ա" << endl;
	}
	return 1;
}
//�����n����������Ϣ

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
//��������

status NameSearchShow(TNode* T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "��������" << endl;
		return 0;
	}

	cout << "������Ϣ:\n" << endl;
	Print(target);
	cout << "\n\n\n��ĸ��Ϣ:\n" << endl;
	Print(target->parent);
	cout << "\n\n\n������Ϣ:\n" << endl;
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
//���������Ϣ

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
//���ղ���

status BirthdateSearchShow(TNode* T, char* bd)
{
	TNode* arr[MAX_SIZE] = { NULL };
	int n = 0;
	BirthdateSearch(T, bd, arr, n);
	cout << "����" << n << "λ��Ա" << endl;
	for (int i = 0; i < n; i++)
	{
		Print(arr[i]);
	}
	return 1;
}
//�������

status FindCommonAncestor(TNode* T, char* na, char* nb)
{
	TNode* a = NameSearch(T, na);
	TNode* b = NameSearch(T, nb);

	if (!a || !b)
	{
		cout << "��������" << endl;
		return -1;
	}

	for (TNode* pa = a; pa != NULL; pa = pa->parent)
	{
		for (TNode* pb = b; pb != NULL; pb = pb->parent)
		{
			if (pa == pb)
			{
				cout << "���˹�ͬ����:" << endl;
				Print(pa);
				return 1;
			}
		}
	}
}
//�������˹�ϵ����ͬ���ȣ�

status AddChild(TNode* T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "�������" << endl;
		return -1;
	}
	cout << "�������º�����Ϣ" << endl;
	TNode* newchild = new TNode;
	
	char name1[20];         //����
	char birthdate1[20];    //��������
	int marry1;            //0  δ��  , 1   �ѻ�
	char address1[50];      //��ַ
	int death1;            //0  ����  , 1   ȥ��
	char deathdate1[20];    //��������

	int sex1;              //0  ��  ,   1  Ů

	cout << "����������" << endl;
	scanf("%s", &name1);

	cout << "�������������" << endl;
	scanf("%s", &birthdate1);

	cout << "�������Ա�(0  �� , 1  Ů)" << endl;
	scanf("%d", &sex1);

	cout << "���������״��(0  δ�� , 1  �ѻ�)" << endl;
	scanf("%d", &marry1);

	cout << "�������ַ" << endl;
	scanf("%s", &address1);

	cout << "�������������(0  ���� , 1  ȥ��)" << endl;
	scanf("%d", &death1);

	if (death1 == 1)
	{
		cout << "��������������" << endl;
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
//���Ӻ���

status DelPerson(TNode*& T, char* n)
{
	TNode* target = NameSearch(T, n);

	if (!target)
	{
		cout << "��������" << endl;
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
//ɾ����Ա

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
//ɾ������ȫ�������ͱ���

status Revise(TNode* &T, char* n)
{
	TNode* target = NameSearch(T, n);
	if (!target)
	{
		cout << "��������" << endl;
		return -1;
	}

	cout << "����������Ϣ" << endl;

	char name1[20];         //����
	char birthdate1[20];    //��������
	int marry1;            //0  δ��  , 1   �ѻ�
	char address1[50];      //��ַ
	int death1;            //0  ����  , 1   ȥ��
	char deathdate1[20];    //��������

	int sex1;              //0  ��  ,   1  Ů

	cout << "����������" << endl;
	scanf("%s", &name1);

	cout << "�������������" << endl;
	scanf("%s", &birthdate1);

	cout << "�������Ա�(0  �� , 1  Ů)" << endl;
	scanf("%d", &sex1);

	cout << "���������״��(0  δ�� , 1  �ѻ�)" << endl;
	scanf("%d", &marry1);

	cout << "�������ַ" << endl;
	scanf("%s", &address1);

	cout << "�������������(0  ���� , 1  ȥ��)" << endl;
	scanf("%d", &death1);

	if (death1 == 1)
	{
		cout << "��������������" << endl;
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

	cout << "�޸���ɣ�" << endl;
	return 1;
}
//�޸���Ϣ

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
	cout << "���ȫ�һ����" << endl;
	return 0;
}
//��ͼ����ʾ

status ShowInGe(TNode* T)
{
	if (T == NULL)
	{
		return -1;
	}

	TNode* p;
	TNode* qu[MAX_SIZE];  // ���У����ڴ洢�ڵ�
	int level[MAX_SIZE];  // �����ж�Ӧ�Ĳ���
	int front = 0, rear = 0;

	qu[rear] = T;         // ���ڵ����
	level[rear] = 1;      // ���ڵ����Ϊ1
	rear++;

	int currentLevel = 1; // ��ǰ����Ĳ���

	while (front != rear)
	{
		p = qu[front];          // ȡ����ͷ�ڵ�
		int nodeLevel = level[front]; // ��ǰ�ڵ����ڲ���
		front = (front + 1) % MAX_SIZE;

		if (nodeLevel > currentLevel) // ������µ�һ��
		{
			cout << endl;       // ����
			currentLevel = nodeLevel;
		}

		printf("%5s (��: %5s)", p->person.name, (p->parent ? p->parent->person.name : "NULL"));

		//cout << p->person.name << "(��ĸ:" << (p->parent ? p->parent->person.name : "NULL") << ")     "; // ����ڵ�����

		p = p->lchild;      // �ӽڵ����
		while (p)
		{
			qu[rear] = p;
			level[rear] = nodeLevel + 1; // �ӽڵ����Ϊ��ǰ����+1
			rear++;
			p = p->rsibling;
		}
	}

	cout << endl; // �����
	return 1;
}
//������

void ShowMenu() 
{
	cout << "\n===== ���׹���ϵͳ =====" << endl;
	cout << "1. ��ʾ����" << endl;
	cout << "2. ��ʾ��N������������Ϣ" << endl;
	cout << "3. ����������ѯ�������Ա��Ϣ�������䱾�ˡ����ס����ӵ���Ϣ��" << endl;
	cout << "4. ���ճ������ڲ�ѯ��Ա����" << endl;
	cout << "5. ��������������ȷ�����ϵ" << endl;
	cout << "6. ĳ��Ա��������" << endl;
	cout << "7. ��ĳ��Ա�������" << endl;
	cout << "8. �޸�ĳ��Ա��Ϣ" << endl;
	cout << "9. �˳�����" << endl;
	cout << "0. ������׵��ļ�" << endl;
	cout << "��ѡ���ܣ�0-9����";
}
//�˵�����

status FilePrint(TNode* T, FILE*& fp2)
{
	fp2 = fopen("familyout.txt", "w");
	if (!fp2)
	{
		cout << "��2�ļ�ʧ�ܣ�" << endl;
		return -1;
	}
	FilePr(T, fp2);

	fclose(fp2);
	cout << "�ɹ������" << endl;
	return 1;

}
//������ļ���

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
//�ļ�����Ӻ���


int main() 
{
	char choice = 0;

	TNode* T=NULL;

	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	
	fp1 = fopen("family.txt", "r");

	if (!fp1)
	{
		cout <<"���ļ�ʧ�ܣ�"<< endl;
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

		// �����û�ѡ����ö�Ӧ����
		switch (choice) 
		{
		case '1':
			cout << endl;
			ShowInPic(T);
			cout << endl;
			ShowInGe(T);
			cout << "\n��ʾ���!" << endl;
			break;
		case '2':
			cout << endl;
			cout << "�������ѯ����" << endl;
			cin >> n;
			cout << endl;
			ShowGeneration(T, n);
			cout << "\n��ʾ���!" << endl;
			break;
		case '3':
			cout << endl;
			cout << "����������" << endl;
			cin >> s;
			cout << endl;
			NameSearchShow(T, s);
			cout << "\n��ʾ���!" << endl;
			break;
		case '4':
			cout << endl;
			cout << "����������" << endl;
			cin >> s;
			cout << endl;
			BirthdateSearchShow(T, s);
			cout << "\n��ʾ���!" << endl;
			break;
		case '5':
			cout << endl;
			cout << "��������������" << endl;
			cin >> s;
			cin >> t;
			cout << endl;
			FindCommonAncestor(T, s, t);
			cout << "\n��ʾ���!" << endl;
			break;
		case '6':
			cout << endl;
			cout << "�����븸������" << endl;
			cin >> s;
			cout << endl;
			AddChild(T, s);
			cout << "\n������!" << endl;
			break;
		case '7':
			cout << endl;
			cout << "�����뱻���������" << endl;
			cin >> s;
			cout << endl;
			DelPerson(T, s);
			cout << "\nɾ�����!" << endl;
			break;
		case '8':
			cout << endl;
			cout << "����������" << endl;
			cin >> s;
			cout << endl;
			Revise(T, s);
			cout << "\n�޸����!" << endl;
			break;
		case '9':
			return 0; // �˳�����
		case '0':
			FilePrint(T, fp2);
			break;
		default:
			cout << endl;
			cout << "��Чѡ�����������룡" << endl;
			break;
		}
	}
	return 0;
}
// ������