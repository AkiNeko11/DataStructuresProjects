#define _CRT_SECURE_NO_WARNINGS 1

/*
9、排序算法比较 （必做）（排序）
[问题描述]
利用随机函数产生10个样本，每个样本有100000个随机整数（并使第一个样本是正序，第二个样本是逆序），利用直接插入排序、希尔排序，冒泡排序、快速排序、选择排序、堆排序，归并排序、基数排序8种排序方法进行排序（结果为由小到大的顺序），并统计每一种排序算法对不同样本所耗费的时间。[要求]
(1) 原始数据存在文件中，用相同样本对不同算法进行测试；
(2) 屏幕显示每种排序算法对不同样本所花的时间；
(3) 任意选择1个样本完成数字排序，即统计出每个整数出现的次数，按出现次数从多到少的顺序输出（输出的信息包含数字和出现的频率）。提示：可用一个数组，以下标作为数，数组内容存储该数出现次数来实现（这就相当于直接映射，是机试题目里面常用的一种解题法，很多看似非线性的题型最后其实都可以采取哈希或者映射的方法来巧解，体会哈希思想在机试题目中的巧用）

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

int dataa[N] = {0};//100000个随机整数

void Createfile()
{
	 
	ofstream file;
	file.open("0.txt", ios::out);
	if (file.fail())
	{
		cout << "文件打开失败\n";
		exit(0);
	}
	if (file)
	{
		for (int i = 0; i <= N; i++)
		{
			dataa[i] = i ;//正序样本
			file << dataa[i] << endl;
		}
	}
	file.close();
	//0.txt,存储正序样本
	
	file.open("1.txt", ios::out);
	if (file.fail())
	{
		cout << "文件打开失败\n";
		exit(0);
	}
	if (file)
	{
		for (int i = 0; i <= N; i++)
		{
			dataa[i] = N - i;//逆序样本
			file << dataa[i] << endl;
		}
	}
	file.close();
	//1.txt，存储逆序样本 


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
			dataa[j] = (rand() % MAX_NUM);//随机样本
			ofile << dataa[j] << endl;
		}
		ofile.close();
	}
	//2-9.txt随机样本
}
//利用随机函数产生10个样本，每个样本有N个随机整数


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
		// 插入到合适的位置
		dataa[j + 1] = temp;
	}
	return 1;
}
//插入排序（每次从原数组中提取一个数字插入已经排好的有序表中）


void InsertionSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	InsertionSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 插入排序" << duration.count() << " 毫秒." << endl;
}
//插入排序计时 


void ShellSort(int dataa[N])
{
	int temp;
	int gap = N / 2;//gap为增量，取总长度的一般
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
		gap = gap / 2;//增量递减
	}
}
//希尔排序


void ShellSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	ShellSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 希尔排序" << duration.count() << " 毫秒." << endl;
}
//希尔排序计时 


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
//冒泡排序


void BubbleSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	BubbleSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 冒泡排序" << duration.count() << " 毫秒." << endl;
}
//冒泡排序计时 



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
//快速排序


void QuickSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	QuickSort(dataa, 0, N - 1);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 快速排序" << duration.count() << " 毫秒." << endl;
}
//快速排序计时 


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
		// 将最小值与第i个元素交换位置
	}
}
//选择排序


void SelectSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	SelectSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 选择排序" << duration.count() << " 毫秒." << endl;
}
//选择排序计时 

 
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
//堆排序 子函数


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
//堆排序,建立大顶堆


void HeapSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	HeapSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 堆排序" << duration.count() << " 毫秒." << endl;
}
//堆排序计时 

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
//归并子函数 


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
//归并排序

void MergeSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	MergeSort(dataa);
	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 归并排序" << duration.count() << " 毫秒." << endl;
}
//归并排序计时 


void RadixSort(int dataa[N])
{
	long int b[N];
	queue<long int> Q[10];
	int radix = 1;
	// 假设最多处理6位数
	for (int i = 0; i < 6; i++)
	{
		// 清空队列
		for (int j = 0; j < 10; j++)
		{
			while (!Q[j].empty())
			{
				Q[j].pop();
			}
		}

		// 计算当前位的基数
		for (int j = 0; j < N; j++)
		{
			b[j] = dataa[j] + 32768; // 偏移以处理负数
			int x = (b[j] / radix) % 10; // 获取当前位的数字
			Q[x].push(b[j]); // 将数字放入对应的队列
		}

		// 收集队列中的数字
		int m = 0;
		for (int j = 0; j < 10; j++)
		{
			while (!Q[j].empty())
			{
				dataa[m++] = Q[j].front() - 32768; // 取出并恢复原值
				Q[j].pop();
			}
		}

		radix *= 10; // 更新基数
	}
}
//基数排序

void RadixSortTime(int dataa[N])
{
	auto start = chrono::high_resolution_clock::now(); // 开始计时

	RadixSort(dataa); // 调用基数排序

	auto end = chrono::high_resolution_clock::now(); // 结束计时
	chrono::duration<double, std::milli> duration = end - start; // 计算耗时

	cout << " 基数排序" << duration.count() << " 毫秒." << endl;
}
//基数排序计时

void sortnum(int dataa[N])
{
	int* frequency = (int*)malloc(sizeof(int) * MAX_NUM); // 用于存储每个数字的出现次数

	for (int i = 0; i < MAX_NUM; i++)
	{
		frequency[i] = 0;
	}

	int count = 0;

	// 统计每个数字的出现次数
	for (int i = 0; i < N; ++i)
	{
		if (dataa[i] >= 0 && dataa[i] <= MAX_NUM) // 确保不越界
		{
			frequency[dataa[i]]++;
		}
	}

	// 创建一个数组来存储频率和对应的数字
	pair<int, int> freq_num[MAX_NUM]; // pair<数字, 频率>
	for (int i = 0; i < MAX_NUM; ++i)
	{
		if (frequency[i] > 0)
		{
			freq_num[count++] = make_pair(i, frequency[i]); // 存储数字和频率
		}
	}

	// 使用简单的冒泡排序按频率从多到少排序
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < count - i; ++j)
		{
			if (freq_num[j].second < freq_num[j + 1].second)
			{
				// 交换频率和对应的数字
				swap(freq_num[j], freq_num[j + 1]);
			}
		}
	}

	// 输出结果
	for (int i = 0; i < count; ++i)
	{
		cout << "数字: " << freq_num[i].first << ", 频率: " << freq_num[i].second << endl;
	}
}
//数字排序 


void get_file(const string name)
{
	ifstream file(name, ios::in);
	for (int i = 0; i < N; i++)
	{
		file >> dataa[i];
	}
}
//文件提取数字


int main()
{
	
	Createfile();

	
	cout << "完成文件0.txt排序:" << endl;
	get_file("0.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件1.txt排序:" << endl;
	get_file("1.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件2.txt排序:" << endl;
	get_file("2.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件3.txt排序:" << endl;
	get_file("3.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件4.txt排序:" << endl;
	get_file("4.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件5.txt排序:" << endl;
	get_file("5.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件6.txt排序:" << endl;
	get_file("6.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件7.txt排序:" << endl;
	get_file("7.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件8.txt排序:" << endl;
	get_file("8.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	cout << "完成文件9.txt排序:" << endl;
	get_file("9.txt");
	InsertionSortTime(dataa);    //插入排序
	ShellSortTime(dataa);        //希尔排序
	BubbleSortTime(dataa);       //冒泡排序
	QuickSortTime(dataa);        //快速排序
	SelectSortTime(dataa);       //选择排序
	HeapSortTime(dataa);         //堆排序
	MergeSortTime(dataa);        //归并排序
	RadixSortTime(dataa);        //基数排序
	cout << endl;

	//选择1个样本完成数字排序,2.txt

	get_file("3.txt");
	sortnum(dataa);
	cout << "完成数字排序，文件3.txt:" << endl;

}
//主函数

