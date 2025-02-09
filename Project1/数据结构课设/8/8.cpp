#define _CRT_SECURE_NO_WARNINGS 1

/*
8、B-树应用 （必做） （查找）
[问题描述]
设计并实现B-树的动态查找。
[基本要求]
（1） 从文件读取数据
（2）实现B-树的三种基本功能：查找、插入和删除。
（3）以可验证的方式输出结果
或者
对1-10000的所有质数，建立m=4的B-tree（每个非叶子结点至少包含1个关键字即2棵子树，最多3个关键字即4棵子树）。
(1)	依次查询200-300的每个数是否在B-tree中，将结果写入文件b-tree1.txt，格式如下:
XXX no
XXX yes
(2)	依次删除500-2000中的每个质数，再查询 600-700之间的每个质数是否在, 将结果写入文件b-tree2.txt，格式如下:
XXX no
XXX yes
(3)	在任务（2）基础上的B-tree，依次插入1-1000的所有偶数，依次查询100-200的每个偶数是否在B-tree中, 将结果写入文件b-tree3.txt，格式如下:
XXX no
XXX yes


*/


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define M 4 // m=4的B树
#define MIN_KEYNUM (M + 1) / 2 - 1//最多3个关键字即4棵子树

//B-树的结构体
typedef struct BTreeNode
{
    int keyNum;               // 结点中关键字的数量
    struct BTreeNode* parent; // 指向双亲结点
    struct Node               // 存放关键字以及其孩子节点指针，正常结点最多存放m个孩子，但在插入判断时会多存放一个
    {
        int key;//关键字
        struct BTreeNode* ptr;//B树当前节点
    } node[M + 1]; // key的0号单元未使用
} BTreeNode, * BTree;

typedef struct Binfo
{
    int win;       // 查找成功的标志
    BTreeNode* pt; // 指向查找到的结点
    int i;         // 结点在关键字中的序号
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

//在m阶B树T上查找关键字K
Binfo Search_key(BTree T, int k)
{
    BTree p = T, q = NULL; //初始化，p指向待查结点，q指向p的双亲 
    int found = 0;
    int index = 0;
    Binfo r;
    while (p && !found)
    {
        index = Search(p, k); // p->node[index].key ≤ K < p->node[index+1].key
        if (index > 0 && p->node[index].key == k)
            found = 1;
        else
        {
            q = p;
            p = p->node[index].ptr;
        }
    }
    r.i = index;
    if (found) // 查找成功
    {
        r.win = 1;//若查找成功，则特征值win=1
        r.pt = p;//指针pt所指结点中第i个关键字等于K
    }
    else
    {
        r.win = 0;//找查失败，则特征值win=0
        r.pt = q;
    }
    return r;
}
//插入关键字
void Insert(BTree* q, int key, BTree ap, int i)
{
    for (int j = (*q)->keyNum; j > i; j--) // 空出(*q)->node[i+1]
    {
        (*q)->node[j + 1] = (*q)->node[j];
    }
    (*q)->node[i + 1].key = key;
    (*q)->node[i + 1].ptr = ap;
    (*q)->keyNum++;
}

// 将结点q分裂成两个结点，mid之前的结点保留，mid之后结点移入新生结点ap
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

// 生成含信息(T,r,ap)的新的根结点&T，原T和ap为子树指针
void NewRoot(BTree* T, int key, BTree ap)
{
    BTree p;
    p = (BTree)malloc(sizeof(BTreeNode));
    p->node[0].ptr = *T; // 根结点孩子数最小为2，则将T作为左孩子，ap作为右孩子
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

// 在m阶B树T上结点*q的key[i]与key[i+1]之间插入关键字K的指针r。若引起结点过大,则沿双亲链进行必要的结点分裂调整,使T仍是m阶B树。
void InseartBTree(BTree* T, int key, BTree q, int i)
{
    BTree ap = NULL;
    int finished = 0;
    int rx = key; // 需要插入的关键字的值
    while (q && !finished)
    {
        Insert(&q, rx, ap, i);
        if (q->keyNum < M)
            finished = 1;
        else // 结点关键字数超出规定
        {
            int mid = (M + 1) / 2; // 结点的中间关键字序号
            rx = q->node[mid].key;
            Split(&q, &ap); // 将q->key[mid+1..M],q->ptr[mid..M]移入新结点*ap
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

//若右兄弟大,则进行左旋操作
void LeftRotation(BTree* q, BTree* p, int i)
{
    // 将父亲结点转移至q结点的末尾
    (*q)->keyNum++;
    (*q)->node[(*q)->keyNum].key = (*p)->node[i + 1].key;
    // 将q结点的右兄弟的第一个关键字转移至父亲结点的分隔符位置
    BTree rightBroPtr = (*p)->node[i + 1].ptr;
    (*p)->node[i + 1].key = rightBroPtr->node[1].key;
    // 将右结点的关键字前移
    for (int j = 1; j < rightBroPtr->keyNum; j++)
    {
        rightBroPtr->node[j] = rightBroPtr->node[j + 1];
    }
    rightBroPtr->keyNum--;
}
//若左兄弟大则进行右旋操作
void RightRotation(BTree* q, BTree* p, int i)
{
    // 将q结点向后移动空出第一个关键字的位置
    for (int j = (*q)->keyNum; j >= 1; j--)
    {
        (*q)->node[j + 1] = (*q)->node[j];
    }
    // 将父亲结点移动至q结点的第一个关键字的位置
    (*q)->node[1].key = (*p)->node[i].key;
    (*q)->node[1].ptr = NULL;
    (*q)->keyNum++;
    // 将左兄弟结点的最后一个关键字移动至父亲结点的分隔符位置
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
        // 将父亲结点的分隔符移动至q结点的最后
        (*q)->keyNum++;
        (*q)->node[(*q)->keyNum].key = (*p)->node[i + 1].key;
        // 将右兄弟结点都移动到q结点上
        (*q)->node[(*q)->keyNum].ptr = rightBroPtr->node[0].ptr;
        for (int j = 1; j <= rightBroPtr->keyNum; j++)
        {
            (*q)->keyNum++;
            (*q)->node[(*q)->keyNum] = rightBroPtr->node[j];
        }
        // 将父亲结点的分隔符删除
        int key = (*p)->node[i + 1].key;
        for (int j = i + 1; j < (*p)->keyNum; j++)
        {
            (*p)->node[j] = (*p)->node[j + 1];
        }
        (*p)->keyNum--;
        if (!(*p)->parent && !(*p)->keyNum)
        {
            // 判断父亲结点是否为根结点，且关键字为空
            // 让q结点作为根结点
            (*q)->parent = NULL;
            (*p) = (*q);
        }
        BalanceCheck(p, key);
    }
    else if (leftBroPtr)
    {
        // 将父亲结点的分隔符移动至左兄弟结点的最后
        leftBroPtr->keyNum++;
        leftBroPtr->node[leftBroPtr->keyNum].key = (*p)->node[i].key;
        // 将q结点都移动到左兄弟结点上
        leftBroPtr->node[leftBroPtr->keyNum].ptr = (*q)->node[0].ptr;
        for (int j = 1; j <= (*q)->keyNum; j++)
        {
            leftBroPtr->keyNum++;
            leftBroPtr->node[leftBroPtr->keyNum] = (*q)->node[j];
        }
        // 将父亲结点的分隔符删除
        int key = (*p)->node[i].key;
        for (int j = i; j < (*p)->keyNum; j++)
        {
            (*p)->node[j] = (*p)->node[j + 1];
        }
        (*p)->keyNum--;
        if (!(*p)->parent && !(*p)->keyNum)
        {
            // 判断父亲结点是否为根结点，且关键字为空
            // 让q结点作为根结点
            (*q)->parent = NULL;
            (*p) = (*q);
        }
        BalanceCheck(p, key);
    }
}
//判断B树是否平衡
void BalanceCheck(BTree* q, int key)
{
    if ((*q)->keyNum < MIN_KEYNUM) // 该结点不满足最小关键字数目要求
    {

        BTree p = (*q)->parent;
        int i = Search(p, key);                                            // 找到q结点在父亲结点中的索引
        if (i + 1 <= p->keyNum && p->node[i + 1].ptr->keyNum > MIN_KEYNUM) // 看q结点的右兄弟是否存在多余结点
        {
            LeftRotation(q, &p, i);
        }
        else if (i - 1 >= 0 && p->node[i - 1].ptr->keyNum > MIN_KEYNUM) // 看q结点的左兄弟是否存在多余结点
        {
            RightRotation(q, &p, i);
        }
        else // q结点的左右兄弟都不存在多余结点
        {
            // 将q结点和其左右兄弟的其中一个以及父亲结点中的分隔符合并
            MergeNode(q, &p, i);
        }
    }
}

//若左右兄弟都不大，则将该结点和其左右兄弟的其中一个以及父亲结点中的分隔符合并
void MergeBro(BTree* left, BTree* right)
{
    if (!(*left)->node[((*left)->keyNum)].ptr)
    {
        // 如果左子树为叶子结点
        (*left)->node[(*left)->keyNum].ptr = (*right)->node[0].ptr;
        for (int j = 1; j <= (*right)->keyNum; j++)
        {
            (*left)->keyNum++;
            (*left)->node[(*left)->keyNum] = (*right)->node[j];
        }
    }
    else
    {
        // 左子树不是叶子结点，则先将左子树最后一个子结点和右子树第一个子结点合并
        MergeBro(&(*left)->node[(*left)->keyNum].ptr, &(*right)->node[0].ptr);
        for (int j = 1; j <= (*right)->keyNum; j++)
        {
            (*left)->keyNum++;
            (*left)->node[(*left)->keyNum] = (*right)->node[j];
        }
    }
    // 合并完对左子树关键字数目进行判断
    if ((*left)->keyNum >= M)
    {
        int mid = (M + 1) / 2; // 结点的中间关键字序号
        int rx = (*left)->node[mid].key;
        BTree ap = NULL;
        Split(&(*left), &ap); // 将q->key[mid+1..M],q->ptr[mid..M]移入新结点*ap
        BTree p = (*left)->parent;
        int i = Search(p, rx);
        Insert(&p, rx, ap, i);
    }
}

//关键字的删除操作
void DeleteBTreeNode(BTree* T, int key)
{
    Binfo res = Search_key(*T, key);
    if (res.win) // 查找成功
    {
        // 判断该结点是否是叶子结点
        if (!res.pt->node[res.i].ptr)
        {
            // 若是叶子结点，则直接删除，然后对该结点进行平衡判断
            Delete(&res.pt, res.i);
            BalanceCheck(&res.pt, key);
        }
        else
        {
            // 若不是叶子节点
            BTree leftChildPtr = res.pt->node[res.i - 1].ptr;
            BTree rightChildPtr = res.pt->node[res.i].ptr;
            if (leftChildPtr->keyNum > MIN_KEYNUM) // 左子树大，则将左子树中提取最大值放到该结点中替换要删除的关键字
            {
                res.pt->node[res.i].key = leftChildPtr->node[leftChildPtr->keyNum].key;
                leftChildPtr->keyNum--;
            }
            else if (rightChildPtr->keyNum > MIN_KEYNUM) // 右子树大，则将右子树中提取最小值放到该结点中替换要删除的关键字
            {
                res.pt->node[res.i].key = rightChildPtr->node[1].key;
                for (int j = 1; j < rightChildPtr->keyNum; j++)
                {
                    rightChildPtr->node[j] = rightChildPtr->node[j + 1];
                }
                rightChildPtr->keyNum--;
            }
            else // 左右子树都不大，则合并左右子树
            {
                MergeBro(&leftChildPtr, &rightChildPtr);
                // 删除结点的关键字
                res.i = Search(res.pt, key); // 合并结点可能会改变结点中关键字的次序，重新查序
                for (int j = res.i; j < res.pt->keyNum; j++)
                {
                    res.pt->node[j] = res.pt->node[j + 1];
                }
                res.pt->keyNum--;
                // 对结点进行平衡判断
                BalanceCheck(&res.pt, key);
            }
        }
    }
}

// 筛法生成质数表
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

// 查询并输出结果到文件
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
    BTree T = nullptr; // 初始化空B树
    const int MAX_PRIME = 10000;
    int primes[MAX_PRIME]; // 固定大小数组存储质数
    int primeCount = 0;

    Binfo c;


    // 生成1-10000的质数表
    generatePrimes(primes, primeCount, MAX_PRIME);

    // 构建B树，将所有质数插入B树
    for (int i = 0; i < primeCount; ++i)
    {
        c = Search_key(T, primes[i]);
        if (!c.win)
        {
            InseartBTree(&T, primes[i], c.pt, c.i);
        }
    }

    // 任务1：查询200-300
    queryToFile(T, 200, 300, "b-tree1.txt");

    // 任务2：删除500-2000的质数并查询600-700的质数
    for (int i = 0; i < primeCount; ++i) 
    {
        if (primes[i] >= 500 && primes[i] <= 2000) 
        {
            DeleteBTreeNode(&T, primes[i]);
        }
    }
    queryToFile(T, 600, 700, "b-tree2.txt");

    // 任务3：插入1-1000的偶数并查询100-200的偶数
    for (int i = 2; i <= 1000; i += 2) 
    {
        c = Search_key(T, i);
        if (!c.win)
        {
            InseartBTree(&T, i, c.pt, c.i);
        }
    }
    queryToFile(T, 100, 200, "b-tree3.txt");

    cout << "任务完成，结果已写入文件。" << endl;
    return 0;
}