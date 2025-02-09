#define _CRT_SECURE_NO_WARNINGS 1

/*
4、 平衡二叉树编程
对于1-10000的质数序列<2，3，5，7，…，9973>，建立平衡二叉排序树。
(1)	依次查询200-300的每个数是否在树中，将结果写入tree1.txt，格式如下:
XXX no
XXX yes
(2)	依次删除500-2000中的每个质数，再查询 600-700之间的每个质数是否在, 将结果写入文件tree2.txt，格式如下:
XXX no
XXX yes
(3)	在任务（2）基础上，依次插入1-1000的所有偶数，依次查询100-200的每个偶数是否在树中, 将结果写入文件tree3.txt，格式如下:
XXX no
XXX yes

*/

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// 节点结构
struct Node 
{
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(NULL), right(NULL) {}
};

// AVL树类
class AVLTree
{
public:
    Node* root;

    AVLTree() : root(NULL) {}

    // 获取节点高度
    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    // 获取平衡因子
    int balanceFactor(Node* node) 
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // 更新节点高度
    void updateHeight(Node* node) 
    {
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    // 右旋
    Node* rotateRight(Node* y) 
    {
        Node* x = y->left;
        Node* T = x->right;

        x->right = y;
        y->left = T;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // 左旋
    Node* rotateLeft(Node* x) 
    {
        Node* y = x->right;
        Node* T = y->left;

        y->left = x;
        x->right = T;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // 平衡节点
    Node* balance(Node* node) 
    {
        updateHeight(node);

        if (balanceFactor(node) > 1) 
        {
            if (balanceFactor(node->left) < 0) 
            {
                node->left = rotateLeft(node->left);//LR
            }
            return rotateRight(node);//LL
        }
        //左

        if (balanceFactor(node) < -1) 
        {
            if (balanceFactor(node->right) > 0) 
            {
                node->right = rotateRight(node->right);//RL
            }
            return rotateLeft(node);//RR
        }
        //右

        return node;
    }

    // 插入节点
    Node* insert(Node* node, int key) 
    {
        if (!node)
        {
            return new Node(key);
        }

        if (key < node->key) 
        {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) 
        {
            node->right = insert(node->right, key);
        }
        else 
        {
            return node; // 不允许重复
        }

        return balance(node);
    }

    // 删除节点
    Node* erase(Node* node, int key) 
    {
        if (!node)
        {
            return NULL;
        }

        if (key < node->key) 
        {
            node->left = erase(node->left, key);
        }
        else if (key > node->key) 
        {
            node->right = erase(node->right, key);
        }
        else 
        {
            if (!node->left || !node->right) 
            {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            else 
            {
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = erase(node->right, temp->key);
            }
        }

        return balance(node);
    }

    // 获取最小值节点
    Node* minValueNode(Node* node) 
    {
        Node* current = node;
        while (current->left) 
        {
            current = current->left;
        }
        return current;
    }

    // 搜索节点
    bool search(Node* node, int key) 
    {
        if (!node)
        {
            return false;
        }

        if (key == node->key)
        {
            return true;
        }

        if (key < node->key)
        {
            return search(node->left, key);
        }
        
        return search(node->right, key);
    }

    void insert(int key) 
    {
        root = insert(root, key);
    }

    void erase(int key) 
    {
        root = erase(root, key);
    }

    bool search(int key) 
    {
        return search(root, key);
    }
};

// 生成质数序列——埃拉托色尼法
int* generatePrimes(int max, int& count) 
{
    bool* is_prime = new bool[max + 1]; 
    for (int i = 0; i <= max; ++i) 
    {
        is_prime[i] = true;
    }

    is_prime[0] = is_prime[1] = false;
    count = 0;

    for (int i = 2; i <= max; ++i) 
    {
        if (is_prime[i]) 
        {
            ++count;
            for (int j = i * 2; j <= max; j += i) 
            {
                is_prime[j] = false;
            }
        }
    }

    int* primes = new int[count];
    int index = 0;
    for (int i = 2; i <= max; ++i) 
    {
        if (is_prime[i]) 
        {
            primes[index++] = i;
            //cout << i << " ";
        }
    }

    delete[] is_prime;
    return primes;
}

// 写入查询结果到文件
void writeResults(const string& filename, int* results, bool* found, int size) 
{
    ofstream file(filename);
    for (int i = 0; i < size; ++i) 
    {
        file << results[i] << " " << (found[i] ? "yes" : "no") << endl;
    }
    file.close();
}

int main() 
{
    AVLTree tree;

    // (1) 构建树
    int primeCount;
    int* primes = generatePrimes(10000, primeCount);
    for (int i = 0; i < primeCount; ++i) 
    {
        tree.insert(primes[i]);
    }

    // (1) 查询200-300的每个数是否在树中
    int results1[101];
    bool found1[101];
    for (int i = 200; i <= 300; ++i) 
    {
        results1[i - 200] = i;
        found1[i - 200] = tree.search(i);
    }
    writeResults("tree1.txt", results1, found1, 101);

    // (2) 删除500-2000之间的每个质数
    for (int i = 0; i < primeCount; ++i) 
    {
        if (primes[i] >= 500 && primes[i] <= 2000) 
        {
            tree.erase(primes[i]);
        }
    }

    // 查询600-700之间的每个质数是否在树中
    int results2[101];
    bool found2[101];
    int index2 = 0;
    for (int i = 0; i < primeCount; ++i) 
    {
        if (primes[i] >= 600 && primes[i] <= 700) 
        {
            results2[index2] = primes[i];
            found2[index2] = tree.search(primes[i]);
            ++index2;
        }
    }
    writeResults("tree2.txt", results2, found2, index2);

    // (3) 插入1-1000的所有偶数
    for (int i = 2; i <= 1000; i += 2) 
    {
        tree.insert(i);
    }

    // 查询100-200的每个偶数是否在树中
    int results3[51];
    bool found3[51];
    for (int i = 100, j = 0; i <= 200; i += 2, ++j) 
    {
        results3[j] = i;
        found3[j] = tree.search(i);
    }
    writeResults("tree3.txt", results3, found3, 51);

    delete[] primes;
    return 0;
}
