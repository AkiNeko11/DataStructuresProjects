#define _CRT_SECURE_NO_WARNINGS 1

/*
5、Huffman编码与解码 (必做)（Huffman编码、二叉树）
[问题描述]
对一篇不少于10000字符的英文文章（source.txt），统计各字符出现的次数，实现Huffman编码(code.dat)，以及对编码结果的解码(recode.txt)。
[基本要求]
（1） 输出每个字符出现的次数和编码,并存储文件(Huffman.txt)。
（2） 在Huffman编码后，英文文章编码结果保存到文件中(code.dat)，编码结果必须是二进制形式，即0 1的信息用比特位表示，不能用字符’0’和’1’表示。
（3） 实现解码功能。

*/


#include <iostream>
#include <fstream>
#include <bitset>                      //bit操作
#include <cstring>
#include <algorithm>                   //sort
using namespace std;

// 定义Huffman树节点
struct Node
{
    char ch;
    int freq;
    Node* left, * right;

    Node(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
};

// 队列结构
struct Queue
{
    struct QueueNode
    {
        Node* data;
        QueueNode* next;
        QueueNode(Node* n) : data(n), next(NULL) {}
    };

    QueueNode* front;
    QueueNode* rear;

    Queue() : front(NULL), rear(NULL) {}

    ~Queue()
    {
        while (front)
        {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void EnQueue(Node* n)
    {
        QueueNode* newNode = new QueueNode(n);
        if (rear)
        {
            rear->next = newNode;
        }
        else
        {
            front = newNode;
        }
        rear = newNode;
    }

    Node* DeQueue()
    {
        if (isEmpty())
        {
            return NULL;
        }

        QueueNode* temp = front;
        front = front->next;

        if (!front)
        {
            rear = NULL;
        }

        Node* node = temp->data;
        delete temp;
        return node;
    }

    Node* peek()
    {
        return isEmpty() ? NULL : front->data;
    }
};

// 合并两个节点的辅助函数
Node* mergeNodes(Node* left, Node* right)
{
    Node* parent = new Node('\0', left->freq + right->freq);    //char='\0'
    parent->left = left;
    parent->right = right;
    return parent;
}

// 生成Huffman编码
void generateCodes(Node* root, const string& str, pair<char, string>* huffmanCode, string* codes, int& index)
{
    if (!root)
    {
        return;
    }

    if (!root->left && !root->right)
    {
        huffmanCode[index] = { root->ch, str };
        index++;
        codes[(unsigned char)root->ch] = str;       //root.ch-char->int
    }
    //叶子结点开始编码

    generateCodes(root->left, str + "0", huffmanCode, codes, index);
    generateCodes(root->right, str + "1", huffmanCode, codes, index);
}

// 释放Huffman树
void freeTree(Node* root)
{
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main()
{
    // 打开输入文件
    ifstream inputFile("source.txt", ios::in);
    if (!inputFile)
    {
        cout << "无法打开source.txt" << endl;
        return 1;
    }

    // 统计字符频率
    int freq[256] = { 0 };
    char ch;
    string text;
    while (inputFile.get(ch))
    {
        freq[(unsigned char)ch]++;     //ch-char->int
        text += ch;
    }
    inputFile.close();

    // 构建Huffman树

    Queue queue;
    //从大到小到queue里，最先进去的最大的，最后进去的最小的
    
    pair<int, int> validData[256];
    int validcount = 0;

    for (int i = 0; i < 256; ++i)
    {
        if (freq[i] > 0)
        {
            validData[validcount] = { freq[i],i };
            validcount++;
        }
    }
    //辅组排序pair

    sort(validData, validData + validcount, [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first; // 比较值大小，按从大到小排序
        });

    for (int i = 0; i < validcount; ++i)
    {
        queue.EnQueue(new Node(validData[i].second, validData[i].first));
    }


    while (queue.front && queue.front->next)
    {
        Node* left = queue.DeQueue();
        Node* right = queue.DeQueue();
        Node* merged = mergeNodes(left, right);
        queue.EnQueue(merged);
    }

    Node* root = queue.DeQueue();

    // 生成Huffman编码
    pair<char, string> huffmanCode[256];     //pair配对，存储字符char及其对应编码string
    string codes[256];                       //以字符值索引，存储字符编码string
    int index = 0;
    generateCodes(root, "", huffmanCode, codes, index);

    // 输出字符频率和编码到Huffman.txt
    ofstream huffmanFile("Huffman.txt", ios::out);
    for (int i = 0; i < index; i++)
    {
        huffmanFile << huffmanCode[i].first << ":频率:  " << freq[(unsigned char)huffmanCode[i].first] << ",编码:  " << huffmanCode[i].second << endl;
    }
    huffmanFile.close();

    // 编码文章并保存到code.dat
    ofstream codeFile("code.dat", ios::binary);
    string encodedText;
    for (char c : text)
    {
        encodedText += codes[(unsigned char)c];
    }

    bitset<8> bits;
    for (size_t i = 0; i < encodedText.size(); i += 8)
    {
        string byte = encodedText.substr(i, 8);
        while (byte.size() < 8) byte += "0";                 // 补齐到8位
        bits = bitset<8>(byte);                              //将byte转化为二进制位           2025.1.3注: 如byte="10010010",则转换为二进制数bits=10010010
        codeFile.put(static_cast<char>(bits.to_ulong()));    //先将二进制数bits转换为无符号整数,再将这个整数强制转换为一个字节char,然后通过二进制输入到文件
    }
    codeFile.close();

    // 解码文件
    ifstream encodedFile("code.dat", ios::binary);
    ofstream decodedFile("recode.txt", ios::out);

    string decodedText;
    Node* current = root;
    while (encodedFile.get(ch))
    {
        bits = bitset<8>(static_cast<unsigned char>(ch));   //将以二进制方式读取的一个字节  ch  转换为二进制位数  bits
        for (int i = 7; i >= 0; --i)
        {
            current = bits.test(i) ? current->right : current->left;    //1往右,0往左
            if (!current->left && !current->right)
            {
                decodedText += current->ch;
                current = root;
            }
        }
    }

    decodedFile << decodedText;
    encodedFile.close();
    decodedFile.close();

    // 释放Huffman树
    freeTree(root);

    cout << "Huffman编码和解码完成！" << endl;
    return 0;
}