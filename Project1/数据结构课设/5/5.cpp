#define _CRT_SECURE_NO_WARNINGS 1

/*
5��Huffman��������� (����)��Huffman���롢��������
[��������]
��һƪ������10000�ַ���Ӣ�����£�source.txt����ͳ�Ƹ��ַ����ֵĴ�����ʵ��Huffman����(code.dat)���Լ��Ա������Ľ���(recode.txt)��
[����Ҫ��]
��1�� ���ÿ���ַ����ֵĴ����ͱ���,���洢�ļ�(Huffman.txt)��
��2�� ��Huffman�����Ӣ�����±��������浽�ļ���(code.dat)�������������Ƕ�������ʽ����0 1����Ϣ�ñ���λ��ʾ���������ַ���0���͡�1����ʾ��
��3�� ʵ�ֽ��빦�ܡ�

*/


#include <iostream>
#include <fstream>
#include <bitset>                      //bit����
#include <cstring>
#include <algorithm>                   //sort
using namespace std;

// ����Huffman���ڵ�
struct Node
{
    char ch;
    int freq;
    Node* left, * right;

    Node(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
};

// ���нṹ
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

// �ϲ������ڵ�ĸ�������
Node* mergeNodes(Node* left, Node* right)
{
    Node* parent = new Node('\0', left->freq + right->freq);    //char='\0'
    parent->left = left;
    parent->right = right;
    return parent;
}

// ����Huffman����
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
    //Ҷ�ӽ�㿪ʼ����

    generateCodes(root->left, str + "0", huffmanCode, codes, index);
    generateCodes(root->right, str + "1", huffmanCode, codes, index);
}

// �ͷ�Huffman��
void freeTree(Node* root)
{
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main()
{
    // �������ļ�
    ifstream inputFile("source.txt", ios::in);
    if (!inputFile)
    {
        cout << "�޷���source.txt" << endl;
        return 1;
    }

    // ͳ���ַ�Ƶ��
    int freq[256] = { 0 };
    char ch;
    string text;
    while (inputFile.get(ch))
    {
        freq[(unsigned char)ch]++;     //ch-char->int
        text += ch;
    }
    inputFile.close();

    // ����Huffman��

    Queue queue;
    //�Ӵ�С��queue����Ƚ�ȥ�����ģ�����ȥ����С��
    
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
    //��������pair

    sort(validData, validData + validcount, [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first; // �Ƚ�ֵ��С�����Ӵ�С����
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

    // ����Huffman����
    pair<char, string> huffmanCode[256];     //pair��ԣ��洢�ַ�char�����Ӧ����string
    string codes[256];                       //���ַ�ֵ�������洢�ַ�����string
    int index = 0;
    generateCodes(root, "", huffmanCode, codes, index);

    // ����ַ�Ƶ�ʺͱ��뵽Huffman.txt
    ofstream huffmanFile("Huffman.txt", ios::out);
    for (int i = 0; i < index; i++)
    {
        huffmanFile << huffmanCode[i].first << ":Ƶ��:  " << freq[(unsigned char)huffmanCode[i].first] << ",����:  " << huffmanCode[i].second << endl;
    }
    huffmanFile.close();

    // �������²����浽code.dat
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
        while (byte.size() < 8) byte += "0";                 // ���뵽8λ
        bits = bitset<8>(byte);                              //��byteת��Ϊ������λ           2025.1.3ע: ��byte="10010010",��ת��Ϊ��������bits=10010010
        codeFile.put(static_cast<char>(bits.to_ulong()));    //�Ƚ���������bitsת��Ϊ�޷�������,�ٽ��������ǿ��ת��Ϊһ���ֽ�char,Ȼ��ͨ�����������뵽�ļ�
    }
    codeFile.close();

    // �����ļ�
    ifstream encodedFile("code.dat", ios::binary);
    ofstream decodedFile("recode.txt", ios::out);

    string decodedText;
    Node* current = root;
    while (encodedFile.get(ch))
    {
        bits = bitset<8>(static_cast<unsigned char>(ch));   //���Զ����Ʒ�ʽ��ȡ��һ���ֽ�  ch  ת��Ϊ������λ��  bits
        for (int i = 7; i >= 0; --i)
        {
            current = bits.test(i) ? current->right : current->left;    //1����,0����
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

    // �ͷ�Huffman��
    freeTree(root);

    cout << "Huffman����ͽ�����ɣ�" << endl;
    return 0;
}