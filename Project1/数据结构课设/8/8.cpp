#define _CRT_SECURE_NO_WARNINGS 1

/*
8��B-��Ӧ�� �������� �����ң�
[��������]
��Ʋ�ʵ��B-���Ķ�̬���ҡ�
[����Ҫ��]
��1�� ���ļ���ȡ����
��2��ʵ��B-�������ֻ������ܣ����ҡ������ɾ����
��3���Կ���֤�ķ�ʽ������
����
��1-10000����������������m=4��B-tree��ÿ����Ҷ�ӽ�����ٰ���1���ؼ��ּ�2�����������3���ؼ��ּ�4����������
(1)	���β�ѯ200-300��ÿ�����Ƿ���B-tree�У������д���ļ�b-tree1.txt����ʽ����:
XXX no
XXX yes
(2)	����ɾ��500-2000�е�ÿ���������ٲ�ѯ 600-700֮���ÿ�������Ƿ���, �����д���ļ�b-tree2.txt����ʽ����:
XXX no
XXX yes
(3)	������2�������ϵ�B-tree�����β���1-1000������ż�������β�ѯ100-200��ÿ��ż���Ƿ���B-tree��, �����д���ļ�b-tree3.txt����ʽ����:
XXX no
XXX yes


*/


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define M 4 // m=4��B��
#define MIN_KEYNUM (M + 1) / 2 - 1//���3���ؼ��ּ�4������

//B-���Ľṹ��
typedef struct BTreeNode
{
    int keyNum;               // ����йؼ��ֵ�����
    struct BTreeNode* parent; // ָ��˫�׽��
    struct Node               // ��Źؼ����Լ��亢�ӽڵ�ָ�룬������������m�����ӣ����ڲ����ж�ʱ�����һ��
    {
        int key;//�ؼ���
        struct BTreeNode* ptr;//B����ǰ�ڵ�
    } node[M + 1]; // key��0�ŵ�Ԫδʹ��
} BTreeNode, * BTree;

typedef struct Binfo
{
    int win;       // ���ҳɹ��ı�־
    BTreeNode* pt; // ָ����ҵ��Ľ��
    int i;         // ����ڹؼ����е����
} Binfo;

int Search(BTree T, int k)
{
    int i = 0;
    for (int j = 1; j <= T->keyNum; j++)
    {
        if (T->node[j].key <= k)
        {
            i = j;
        }
    }
    return i;
}

//��m��B��T�ϲ��ҹؼ���K
Binfo Search_key(BTree T, int k)
{
    BTree p = T, q = NULL; //��ʼ����pָ������㣬qָ��p��˫�� 
    int found = 0;
    int index = 0;
    Binfo r;
    while (p && !found)
    {
        index = Search(p, k); // p->node[index].key �� K < p->node[index+1].key
        if (index > 0 && p->node[index].key == k)
            found = 1;
        else
        {
            q = p;
            p = p->node[index].ptr;
        }
    }
    r.i = index;
    if (found) // ���ҳɹ�
    {
        r.win = 1;//�����ҳɹ���������ֵwin=1
        r.pt = p;//ָ��pt��ָ����е�i���ؼ��ֵ���K
    }
    else
    {
        r.win = 0;//�Ҳ�ʧ�ܣ�������ֵwin=0
        r.pt = q;
    }
    return r;
}
//����ؼ���
void Insert(BTree* q, int key, BTree ap, int i)
{
    for (int j = (*q)->keyNum; j > i; j--) // �ճ�(*q)->node[i+1]
    {
        (*q)->node[j + 1] = (*q)->node[j];
    }
    (*q)->node[i + 1].key = key;
    (*q)->node[i + 1].ptr = ap;
    (*q)->keyNum++;
}

// �����q���ѳ�������㣬mid֮ǰ�Ľ�㱣����mid֮���������������ap
void Split(BTree* q, BTree* ap)
{
    int mid = (M + 1) / 2;
    *ap = (BTree)malloc(sizeof(BTreeNode));
    (*ap)->node[0].ptr = (*q)->node[mid].ptr;
    if ((*ap)->node[0].ptr)
    {
        (*ap)->node[0].ptr->parent = *ap;
    }

    for (int i = mid + 1; i <= M; i++)
    {
        (*ap)->node[i - mid] = (*q)->node[i];
        if ((*ap)->node[i - mid].ptr)
        {
            (*ap)->node[i - mid].ptr->parent = *ap;
        }
    }
    (*ap)->keyNum = M - mid;
    (*ap)->parent = (*q)->parent;
    (*q)->keyNum = mid - 1;
}

// ���ɺ���Ϣ(T,r,ap)���µĸ����&T��ԭT��apΪ����ָ��
void NewRoot(BTree* T, int key, BTree ap)
{
    BTree p;
    p = (BTree)malloc(sizeof(BTreeNode));
    p->node[0].ptr = *T; // ����㺢������СΪ2����T��Ϊ���ӣ�ap��Ϊ�Һ���
    *T = p;
    if ((*T)->node[0].ptr)
    {
        (*T)->node[0].ptr->parent = *T;
    }
    (*T)->parent = NULL;
    (*T)->keyNum = 1;
    (*T)->node[1].key = key;
    (*T)->node[1].ptr = ap;
    if ((*T)->node[1].ptr)
    {
        (*T)->node[1].ptr->parent = *T;
    }
}

// ��m��B��T�Ͻ��*q��key[i]��key[i+1]֮�����ؼ���K��ָ��r�������������,����˫�������б�Ҫ�Ľ����ѵ���,ʹT����m��B����
void InseartBTree(BTree* T, int key, BTree q, int i)
{
    BTree ap = NULL;
    int finished = 0;
    int rx = key; // ��Ҫ����Ĺؼ��ֵ�ֵ
    while (q && !finished)
    {
        Insert(&q, rx, ap, i);
        if (q->keyNum < M)
            finished = 1;
        else // ���ؼ����������涨
        {
            int mid = (M + 1) / 2; // �����м�ؼ������
            rx = q->node[mid].key;
            Split(&q, &ap); // ��q->key[mid+1..M],q->ptr[mid..M]�����½��*ap
            q = q->parent;
            if (q)
                i = Search(q, rx);
        }
    }
    if (!finished)
    {
        NewRoot(T, rx, ap);
    }
}

void Delete(BTree* q, int index)
{
    for (int i = index; i <= (*q)->keyNum; i++)
    {
        (*q)->node[index] = (*q)->node[index + 1];
    }
    (*q)->keyNum--;
}

//�����ֵܴ�,�������������
void LeftRotation(BTree* q, BTree* p, int i)
{
    // �����׽��ת����q����ĩβ
    (*q)->keyNum++;
    (*q)->node[(*q)->keyNum].key = (*p)->node[i + 1].key;
    // ��q�������ֵܵĵ�һ���ؼ���ת�������׽��ķָ���λ��
    BTree rightBroPtr = (*p)->node[i + 1].ptr;
    (*p)->node[i + 1].key = rightBroPtr->node[1].key;
    // ���ҽ��Ĺؼ���ǰ��
    for (int j = 1; j < rightBroPtr->keyNum; j++)
    {
        rightBroPtr->node[j] = rightBroPtr->node[j + 1];
    }
    rightBroPtr->keyNum--;
}
//�����ֵܴ��������������
void RightRotation(BTree* q, BTree* p, int i)
{
    // ��q�������ƶ��ճ���һ���ؼ��ֵ�λ��
    for (int j = (*q)->keyNum; j >= 1; j--)
    {
        (*q)->node[j + 1] = (*q)->node[j];
    }
    // �����׽���ƶ���q���ĵ�һ���ؼ��ֵ�λ��
    (*q)->node[1].key = (*p)->node[i].key;
    (*q)->node[1].ptr = NULL;
    (*q)->keyNum++;
    // �����ֵܽ������һ���ؼ����ƶ������׽��ķָ���λ��
    BTree leftBroPtr = (*p)->node[i - 1].ptr;
    (*p)->node[i].key = leftBroPtr->node[leftBroPtr->keyNum].key;
    leftBroPtr->keyNum--;
}

void BalanceCheck(BTree* q, int key);

void MergeNode(BTree* q, BTree* p, int i)
{
    BTree rightBroPtr = NULL, leftBroPtr = NULL;
    if (i + 1 <= (*p)->keyNum)
    {
        rightBroPtr = (*p)->node[i + 1].ptr;
    }
    if (i - 1 >= 0)
    {
        leftBroPtr = (*p)->node[i - 1].ptr;
    }
    if (rightBroPtr)
    {
        // �����׽��ķָ����ƶ���q�������
        (*q)->keyNum++;
        (*q)->node[(*q)->keyNum].key = (*p)->node[i + 1].key;
        // �����ֵܽ�㶼�ƶ���q�����
        (*q)->node[(*q)->keyNum].ptr = rightBroPtr->node[0].ptr;
        for (int j = 1; j <= rightBroPtr->keyNum; j++)
        {
            (*q)->keyNum++;
            (*q)->node[(*q)->keyNum] = rightBroPtr->node[j];
        }
        // �����׽��ķָ���ɾ��
        int key = (*p)->node[i + 1].key;
        for (int j = i + 1; j < (*p)->keyNum; j++)
        {
            (*p)->node[j] = (*p)->node[j + 1];
        }
        (*p)->keyNum--;
        if (!(*p)->parent && !(*p)->keyNum)
        {
            // �жϸ��׽���Ƿ�Ϊ����㣬�ҹؼ���Ϊ��
            // ��q�����Ϊ�����
            (*q)->parent = NULL;
            (*p) = (*q);
        }
        BalanceCheck(p, key);
    }
    else if (leftBroPtr)
    {
        // �����׽��ķָ����ƶ������ֵܽ������
        leftBroPtr->keyNum++;
        leftBroPtr->node[leftBroPtr->keyNum].key = (*p)->node[i].key;
        // ��q��㶼�ƶ������ֵܽ����
        leftBroPtr->node[leftBroPtr->keyNum].ptr = (*q)->node[0].ptr;
        for (int j = 1; j <= (*q)->keyNum; j++)
        {
            leftBroPtr->keyNum++;
            leftBroPtr->node[leftBroPtr->keyNum] = (*q)->node[j];
        }
        // �����׽��ķָ���ɾ��
        int key = (*p)->node[i].key;
        for (int j = i; j < (*p)->keyNum; j++)
        {
            (*p)->node[j] = (*p)->node[j + 1];
        }
        (*p)->keyNum--;
        if (!(*p)->parent && !(*p)->keyNum)
        {
            // �жϸ��׽���Ƿ�Ϊ����㣬�ҹؼ���Ϊ��
            // ��q�����Ϊ�����
            (*q)->parent = NULL;
            (*p) = (*q);
        }
        BalanceCheck(p, key);
    }
}
//�ж�B���Ƿ�ƽ��
void BalanceCheck(BTree* q, int key)
{
    if ((*q)->keyNum < MIN_KEYNUM) // �ý�㲻������С�ؼ�����ĿҪ��
    {

        BTree p = (*q)->parent;
        int i = Search(p, key);                                            // �ҵ�q����ڸ��׽���е�����
        if (i + 1 <= p->keyNum && p->node[i + 1].ptr->keyNum > MIN_KEYNUM) // ��q�������ֵ��Ƿ���ڶ�����
        {
            LeftRotation(q, &p, i);
        }
        else if (i - 1 >= 0 && p->node[i - 1].ptr->keyNum > MIN_KEYNUM) // ��q�������ֵ��Ƿ���ڶ�����
        {
            RightRotation(q, &p, i);
        }
        else // q���������ֵܶ������ڶ�����
        {
            // ��q�����������ֵܵ�����һ���Լ����׽���еķָ����ϲ�
            MergeNode(q, &p, i);
        }
    }
}

//�������ֵܶ������򽫸ý����������ֵܵ�����һ���Լ����׽���еķָ����ϲ�
void MergeBro(BTree* left, BTree* right)
{
    if (!(*left)->node[((*left)->keyNum)].ptr)
    {
        // ���������ΪҶ�ӽ��
        (*left)->node[(*left)->keyNum].ptr = (*right)->node[0].ptr;
        for (int j = 1; j <= (*right)->keyNum; j++)
        {
            (*left)->keyNum++;
            (*left)->node[(*left)->keyNum] = (*right)->node[j];
        }
    }
    else
    {
        // ����������Ҷ�ӽ�㣬���Ƚ����������һ���ӽ�����������һ���ӽ��ϲ�
        MergeBro(&(*left)->node[(*left)->keyNum].ptr, &(*right)->node[0].ptr);
        for (int j = 1; j <= (*right)->keyNum; j++)
        {
            (*left)->keyNum++;
            (*left)->node[(*left)->keyNum] = (*right)->node[j];
        }
    }
    // �ϲ�����������ؼ�����Ŀ�����ж�
    if ((*left)->keyNum >= M)
    {
        int mid = (M + 1) / 2; // �����м�ؼ������
        int rx = (*left)->node[mid].key;
        BTree ap = NULL;
        Split(&(*left), &ap); // ��q->key[mid+1..M],q->ptr[mid..M]�����½��*ap
        BTree p = (*left)->parent;
        int i = Search(p, rx);
        Insert(&p, rx, ap, i);
    }
}

//�ؼ��ֵ�ɾ������
void DeleteBTreeNode(BTree* T, int key)
{
    Binfo res = Search_key(*T, key);
    if (res.win) // ���ҳɹ�
    {
        // �жϸý���Ƿ���Ҷ�ӽ��
        if (!res.pt->node[res.i].ptr)
        {
            // ����Ҷ�ӽ�㣬��ֱ��ɾ����Ȼ��Ըý�����ƽ���ж�
            Delete(&res.pt, res.i);
            BalanceCheck(&res.pt, key);
        }
        else
        {
            // ������Ҷ�ӽڵ�
            BTree leftChildPtr = res.pt->node[res.i - 1].ptr;
            BTree rightChildPtr = res.pt->node[res.i].ptr;
            if (leftChildPtr->keyNum > MIN_KEYNUM) // ��������������������ȡ���ֵ�ŵ��ý�����滻Ҫɾ���Ĺؼ���
            {
                res.pt->node[res.i].key = leftChildPtr->node[leftChildPtr->keyNum].key;
                leftChildPtr->keyNum--;
            }
            else if (rightChildPtr->keyNum > MIN_KEYNUM) // ��������������������ȡ��Сֵ�ŵ��ý�����滻Ҫɾ���Ĺؼ���
            {
                res.pt->node[res.i].key = rightChildPtr->node[1].key;
                for (int j = 1; j < rightChildPtr->keyNum; j++)
                {
                    rightChildPtr->node[j] = rightChildPtr->node[j + 1];
                }
                rightChildPtr->keyNum--;
            }
            else // ����������������ϲ���������
            {
                MergeBro(&leftChildPtr, &rightChildPtr);
                // ɾ�����Ĺؼ���
                res.i = Search(res.pt, key); // �ϲ������ܻ�ı����йؼ��ֵĴ������²���
                for (int j = res.i; j < res.pt->keyNum; j++)
                {
                    res.pt->node[j] = res.pt->node[j + 1];
                }
                res.pt->keyNum--;
                // �Խ�����ƽ���ж�
                BalanceCheck(&res.pt, key);
            }
        }
    }
}

// ɸ������������
void generatePrimes(int* primes, int& count, int max)
{
    bool* isPrime = new bool[max + 1];
    memset(isPrime, true, (max + 1) * sizeof(bool));
    isPrime[0] = isPrime[1] = false;
    count = 0;

    for (int i = 2; i <= sqrt(max); ++i) 
    {
        if (isPrime[i]) {
            for (int j = i * i; j <= max; j += i) 
            {
                isPrime[j] = false;
            }
        }
    }
    for (int i = 2; i <= max; ++i) 
    {
        if (isPrime[i]) {
            primes[count++] = i;
        }
    }
    delete[] isPrime;
}

// ��ѯ�����������ļ�
void queryToFile(BTree T, int start, int end, const char* filename)
{
    ofstream outFile(filename);
    for (int num = start; num <= end; ++num)
    {
        Binfo res = Search_key(T, num);
        if (res.win) 
        {
            outFile << num << " yes\n";
        }
        else 
        {
            outFile << num << " no\n";
        }
    }
    outFile.close();
}

int main()
{
    BTree T = nullptr; // ��ʼ����B��
    const int MAX_PRIME = 10000;
    int primes[MAX_PRIME]; // �̶���С����洢����
    int primeCount = 0;

    Binfo c;


    // ����1-10000��������
    generatePrimes(primes, primeCount, MAX_PRIME);

    // ����B������������������B��
    for (int i = 0; i < primeCount; ++i)
    {
        c = Search_key(T, primes[i]);
        if (!c.win)
        {
            InseartBTree(&T, primes[i], c.pt, c.i);
        }
    }

    // ����1����ѯ200-300
    queryToFile(T, 200, 300, "b-tree1.txt");

    // ����2��ɾ��500-2000����������ѯ600-700������
    for (int i = 0; i < primeCount; ++i) 
    {
        if (primes[i] >= 500 && primes[i] <= 2000) 
        {
            DeleteBTreeNode(&T, primes[i]);
        }
    }
    queryToFile(T, 600, 700, "b-tree2.txt");

    // ����3������1-1000��ż������ѯ100-200��ż��
    for (int i = 2; i <= 1000; i += 2) 
    {
        c = Search_key(T, i);
        if (!c.win)
        {
            InseartBTree(&T, i, c.pt, c.i);
        }
    }
    queryToFile(T, 100, 200, "b-tree3.txt");

    cout << "������ɣ������д���ļ���" << endl;
    return 0;
}