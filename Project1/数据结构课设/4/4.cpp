#define _CRT_SECURE_NO_WARNINGS 1

/*
4�� ƽ����������
����1-10000����������<2��3��5��7������9973>������ƽ�������������
(1)	���β�ѯ200-300��ÿ�����Ƿ������У������д��tree1.txt����ʽ����:
XXX no
XXX yes
(2)	����ɾ��500-2000�е�ÿ���������ٲ�ѯ 600-700֮���ÿ�������Ƿ���, �����д���ļ�tree2.txt����ʽ����:
XXX no
XXX yes
(3)	������2�������ϣ����β���1-1000������ż�������β�ѯ100-200��ÿ��ż���Ƿ�������, �����д���ļ�tree3.txt����ʽ����:
XXX no
XXX yes

*/

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

// �ڵ�ṹ
struct Node 
{
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(NULL), right(NULL) {}
};

// AVL����
class AVLTree
{
public:
    Node* root;

    AVLTree() : root(NULL) {}

    // ��ȡ�ڵ�߶�
    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    // ��ȡƽ������
    int balanceFactor(Node* node) 
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // ���½ڵ�߶�
    void updateHeight(Node* node) 
    {
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    // ����
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

    // ����
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

    // ƽ��ڵ�
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
        //��

        if (balanceFactor(node) < -1) 
        {
            if (balanceFactor(node->right) > 0) 
            {
                node->right = rotateRight(node->right);//RL
            }
            return rotateLeft(node);//RR
        }
        //��

        return node;
    }

    // ����ڵ�
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
            return node; // �������ظ�
        }

        return balance(node);
    }

    // ɾ���ڵ�
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

    // ��ȡ��Сֵ�ڵ�
    Node* minValueNode(Node* node) 
    {
        Node* current = node;
        while (current->left) 
        {
            current = current->left;
        }
        return current;
    }

    // �����ڵ�
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

// �����������С���������ɫ�ᷨ
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

// д���ѯ������ļ�
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

    // (1) ������
    int primeCount;
    int* primes = generatePrimes(10000, primeCount);
    for (int i = 0; i < primeCount; ++i) 
    {
        tree.insert(primes[i]);
    }

    // (1) ��ѯ200-300��ÿ�����Ƿ�������
    int results1[101];
    bool found1[101];
    for (int i = 200; i <= 300; ++i) 
    {
        results1[i - 200] = i;
        found1[i - 200] = tree.search(i);
    }
    writeResults("tree1.txt", results1, found1, 101);

    // (2) ɾ��500-2000֮���ÿ������
    for (int i = 0; i < primeCount; ++i) 
    {
        if (primes[i] >= 500 && primes[i] <= 2000) 
        {
            tree.erase(primes[i]);
        }
    }

    // ��ѯ600-700֮���ÿ�������Ƿ�������
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

    // (3) ����1-1000������ż��
    for (int i = 2; i <= 1000; i += 2) 
    {
        tree.insert(i);
    }

    // ��ѯ100-200��ÿ��ż���Ƿ�������
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
