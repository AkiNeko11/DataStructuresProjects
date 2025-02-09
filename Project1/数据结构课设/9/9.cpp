#define _CRT_SECURE_NO_WARNINGS 1

/*
9�������㷨�Ƚ� ��������������
[��������]
���������������10��������ÿ��������100000�������������ʹ��һ�����������򣬵ڶ������������򣩣�����ֱ�Ӳ�������ϣ������ð�����򡢿�������ѡ�����򡢶����򣬹鲢���򡢻�������8�����򷽷��������򣨽��Ϊ��С�����˳�򣩣���ͳ��ÿһ�������㷨�Բ�ͬ�������ķѵ�ʱ�䡣[Ҫ��]
(1) ԭʼ���ݴ����ļ��У�����ͬ�����Բ�ͬ�㷨���в��ԣ�
(2) ��Ļ��ʾÿ�������㷨�Բ�ͬ����������ʱ�䣻
(3) ����ѡ��1����������������򣬼�ͳ�Ƴ�ÿ���������ֵĴ����������ִ����Ӷൽ�ٵ�˳��������������Ϣ�������ֺͳ��ֵ�Ƶ�ʣ�����ʾ������һ�����飬���±���Ϊ�����������ݴ洢�������ִ�����ʵ�֣�����൱��ֱ��ӳ�䣬�ǻ�����Ŀ���泣�õ�һ�ֽ��ⷨ���ܶ࿴�Ʒ����Ե����������ʵ�����Բ�ȡ��ϣ����ӳ��ķ������ɽ⣬����ϣ˼���ڻ�����Ŀ�е����ã�

*/


#include <iostream>
#include <time.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <chrono>
#include <utility> 
#include <algorithm>
#include <queue>
using namespace std;
#define OK 1
#define N 100000
#define MAX_NUM 30000

int dataa[N] = {0};//100000���������

void Createfile()
{
	 
	ofstream file;
	file.open("0.txt", ios::out);
	if (file.fail())
	{
		cout << "�ļ���ʧ��\n";
		exit(0);
	}
	if (file)
	{
		for (int i = 0; i <= N; i++)
		{
			dataa[i] = i ;//��������
			file << dataa[i] << endl;
		}
	}
	file.close();
	//0.txt,�洢��������
	
	file.open("1.txt", ios::out);
	if (file.fail())
	{
		cout << "�ļ���ʧ��\n";
		exit(0);
	}
	if (file)
	{
		for (int i = 0; i <= N; i++)
		{
			dataa[i] = N - i;//��������
			file << dataa[i] << endl;
		}
	}
	file.close();
	//1.txt���洢�������� 


	srand(time(NULL));
	for (char i = '2'; i <= '9'; i++)
	{
		string filename;
		stringstream s;
		s << i;
		s >> filename;
		filename = filename + ".txt";
		char file[10];
		strcpy(file, filename.c_str());
		ofstream ofile;
		ofile.open(file);

		for (int j = 0; j <= N; j++)
		{
			dataa[j] = (rand() % MAX_NUM);//�������
			ofile << dataa[j] << endl;
		}
		ofile.close();
	}
	//2-9.txt�������
}
//���������������10��������ÿ��������N���������


int InsertionSort(int dataa[N])
{
	int temp;
	for (int i = 1; i < N; i++)
	{
		temp = dataa[i];
		int j = i - 1;
		while (j >= 0 && dataa[j] > temp)
		{
			dataa[j + 1] = dataa[j];
			j--;
		}
		// ���뵽���ʵ�λ��
		dataa[j + 1] = temp;
	}
	return 1;
}
//��������ÿ�δ�ԭ��������ȡһ�����ֲ����Ѿ��źõ�������У�


void InsertionSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	InsertionSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ��������" << duration.count() << " ����." << endl;
}
//���������ʱ 


void ShellSort(int dataa[N])
{
	int temp;
	int gap = N / 2;//gapΪ������ȡ�ܳ��ȵ�һ��
	while (gap >= 1)
	{
		for (int i = 0; i < gap; i++)
		{
			for (int j = i + gap; j < N; j = j + gap)
			{
				if (dataa[j] < dataa[j - gap])
				{
					int k = j - gap;
					temp = dataa[j];
					while (k >= 0 && dataa[k] > temp)
					{
						dataa[k + gap] = dataa[k];
						k = k - gap;
					}
					dataa[k + gap] = temp;
				}
			}
		}
		gap = gap / 2;//�����ݼ�
	}
}
//ϣ������


void ShellSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	ShellSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ϣ������" << duration.count() << " ����." << endl;
}
//ϣ�������ʱ 


int BubbleSort(int dataa[N])
{
	for (int i = 0; i < N ; i++)
	{
		for (int j = 0; j < N - i -1 ; j++)
		{
			if (dataa[j] > dataa[j + 1])
			{
				int temp = dataa[j];
				dataa[j] = dataa[j + 1];
				dataa[j + 1] = temp;
			}
		}
	}
	return 1;
}
//ð������


void BubbleSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	BubbleSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ð������" << duration.count() << " ����." << endl;
}
//ð�������ʱ 



int QuickSort(int dataa[], int low, int high)
{
	if (low > high)
	{
		return OK;
	}
	int i = low, j = high;
	int k = dataa[(low + high) / 2];
	while (i <= j)
	{
		while (dataa[j] > k)
		{
			j--;
		}
		while (dataa[i] < k)
		{
			i++;
		}
		if (i <= j)
		{
			int temp;
			temp = dataa[i];
			dataa[i] = dataa[j];
			dataa[j] = temp;
			i++;
			j--;
		}
	}
	if (low < j)
	{
		QuickSort(dataa, low, j);
	}
	if (high > i)
	{
		QuickSort(dataa, i, high);
	}
}
//��������


void QuickSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	QuickSort(dataa, 0, N - 1);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ��������" << duration.count() << " ����." << endl;
}
//���������ʱ 


void SelectSort(int dataa[50000])
{
	for (int i = 0; i < N - 1; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			if (dataa[i] > dataa[j])
			{
				int temp = dataa[i];
				dataa[i] = dataa[j];
				dataa[j] = temp;
			}
		}
		// ����Сֵ���i��Ԫ�ؽ���λ��
	}
}
//ѡ������


void SelectSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	SelectSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ѡ������" << duration.count() << " ����." << endl;
}
//ѡ�������ʱ 

 
void AdjustHeap(int dataa[N], int low, int high)
{
	int i = low, j = 2 * i;
	int temp = dataa[low];
	while (j <= high)
	{
		if (j + 1 < high && dataa[j] < dataa[j + 1])
		{
			j++;
		}
		if (temp >= dataa[j])
		{
			break;
		}
		dataa[i] = dataa[j];
		i = j;
		j = 2 * i;
	}
	dataa[i] = temp;
}
//������ �Ӻ���


void HeapSort(int dataa[N])
{
	for (int i = (N / 2) - 1; i >= 0; i--)
	{
		AdjustHeap(dataa, i, N - 1);
	}
	for (int i = N - 1; i > 0; i--)
	{
		int temp;
		temp = dataa[i];
		dataa[i] = dataa[0];
		dataa[0] = temp;
		AdjustHeap(dataa, 0, i - 1);
	}
}
//������,�����󶥶�


void HeapSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	HeapSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ������" << duration.count() << " ����." << endl;
}
//�������ʱ 

int dataa1[N];

void Merge(int dataa[N], int low, int mid, int high)
{
	int i = low, j = mid + 1;
	int k = 0;
	while (i <= mid && j <= high)
	{
		if (dataa[i] <= dataa[j])
		{
			dataa1[k++] = dataa[i++];
		}
		else
		{
			dataa1[k++] = dataa[j++];
		}
	}
	while (i <= mid)
	{
		dataa1[k++] = dataa[i++];
	}
	while (j <= high)
	{
		dataa1[k++] = dataa[j++];
	}
	k = 0;
	for (int i = low; i <= high; i++)
	{
		dataa[i] = dataa1[k];
		k++;
	}
}
//�鲢�Ӻ��� 


int MergeSort(int dataa[N])
{
	for (int len = 1; len <= N; len = len * 2)
	{
		int i = 0;
		while (i + 2 * len <= N)
		{
			Merge(dataa, i, i + len - 1, i + 2 * len - 1);
			i = i + 2 * len;
		}
		if (i + len < N)
		{
			Merge(dataa, i, i + len - 1, N - 1);
		}
	}
	return 1;
}
//�鲢����

void MergeSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	MergeSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " �鲢����" << duration.count() << " ����." << endl;
}
//�鲢�����ʱ 


void RadixSort(int dataa[N])
{
	long int b[N];
	queue<long int> Q[10];
	int radix = 1;
	// ������ദ��6λ��
	for (int i = 0; i < 6; i++)
	{
		// ��ն���
		for (int j = 0; j < 10; j++)
		{
			while (!Q[j].empty())
			{
				Q[j].pop();
			}
		}

		// ���㵱ǰλ�Ļ���
		for (int j = 0; j < N; j++)
		{
			b[j] = dataa[j] + 32768; // ƫ���Դ�����
			int x = (b[j] / radix) % 10; // ��ȡ��ǰλ������
			Q[x].push(b[j]); // �����ַ����Ӧ�Ķ���
		}

		// �ռ������е�����
		int m = 0;
		for (int j = 0; j < 10; j++)
		{
			while (!Q[j].empty())
			{
				dataa[m++] = Q[j].front() - 32768; // ȡ�����ָ�ԭֵ
				Q[j].pop();
			}
		}

		radix *= 10; // ���»���
	}
}
//��������

void RadixSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // ��ʼ��ʱ

	RadixSort(dataa); // ���û�������

	auto end = chrono::high_resolution_clock::now(); // ������ʱ
	chrono::duration<double, std::milli> duration = end - start; // �����ʱ

	cout << " ��������" << duration.count() << " ����." << endl;
}
//���������ʱ

void sortnum(int dataa[N])
{
	int* frequency = (int*)malloc(sizeof(int) * MAX_NUM); // ���ڴ洢ÿ�����ֵĳ��ִ���

	for (int i = 0; i < MAX_NUM; i++)
	{
		frequency[i] = 0;
	}

	int count = 0;

	// ͳ��ÿ�����ֵĳ��ִ���
	for (int i = 0; i < N; ++i)
	{
		if (dataa[i] >= 0 && dataa[i] <= MAX_NUM) // ȷ����Խ��
		{
			frequency[dataa[i]]++;
		}
	}

	// ����һ���������洢Ƶ�ʺͶ�Ӧ������
	pair<int, int> freq_num[MAX_NUM]; // pair<����, Ƶ��>
	for (int i = 0; i < MAX_NUM; ++i)
	{
		if (frequency[i] > 0)
		{
			freq_num[count++] = make_pair(i, frequency[i]); // �洢���ֺ�Ƶ��
		}
	}

	// ʹ�ü򵥵�ð������Ƶ�ʴӶൽ������
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < count - i; ++j)
		{
			if (freq_num[j].second < freq_num[j + 1].second)
			{
				// ����Ƶ�ʺͶ�Ӧ������
				swap(freq_num[j], freq_num[j + 1]);
			}
		}
	}

	// ������
	for (int i = 0; i < count; ++i)
	{
		cout << "����: " << freq_num[i].first << ", Ƶ��: " << freq_num[i].second << endl;
	}
}
//�������� 


void get_file(const string name)
{
	ifstream file(name, ios::in);
	for (int i = 0; i < N; i++)
	{
		file >> dataa[i];
	}
}
//�ļ���ȡ����


int main()
{
	
	Createfile();

	
	cout << "����ļ�0.txt����:" << endl;
	get_file("0.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�1.txt����:" << endl;
	get_file("1.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�2.txt����:" << endl;
	get_file("2.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�3.txt����:" << endl;
	get_file("3.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�4.txt����:" << endl;
	get_file("4.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�5.txt����:" << endl;
	get_file("5.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�6.txt����:" << endl;
	get_file("6.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�7.txt����:" << endl;
	get_file("7.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�8.txt����:" << endl;
	get_file("8.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	cout << "����ļ�9.txt����:" << endl;
	get_file("9.txt");
	InsertionSortTime(dataa);    //��������
	ShellSortTime(dataa);        //ϣ������
	BubbleSortTime(dataa);       //ð������
	QuickSortTime(dataa);        //��������
	SelectSortTime(dataa);       //ѡ������
	HeapSortTime(dataa);         //������
	MergeSortTime(dataa);        //�鲢����
	RadixSortTime(dataa);        //��������
	cout << endl;

	//ѡ��1�����������������,2.txt

	get_file("3.txt");
	sortnum(dataa);
	cout << "������������ļ�3.txt:" << endl;

}
//������

