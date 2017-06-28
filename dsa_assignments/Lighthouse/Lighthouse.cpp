// Lighthouse.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <cstdio>
#include <stdio.h>
#include <cassert>
#include <cstdlib>

//��ֹredirecting
//#define _OJ_

// ����д�ģ������ο���
const int SZ = 1 << 20;  //����io  
struct fastio {
	char inbuf[SZ];
	char outbuf[SZ];
	fastio() {
		setvbuf(stdin, inbuf, _IOFBF, SZ);
		setvbuf(stdout, outbuf, _IOFBF, SZ);
	}
}io;

//test case generator����������
void generateTextCases(int num_houses) {
	freopen("input.txt", "w", stdout);
	for (int i = 0; i < num_houses; i++)
	{
		srand(i);
		printf("%d %d\n", rand() % 1000, rand() % 1000);
	}
}



//��·�鲢��ʱ�临�Ӷ�O(n),�ռ临�Ӷ�O(mi - lo)
void merge(int(*arr)[2], int lo, int mi, int hi) {
	int(*A)[2] = arr + lo; //�ϲ��������A[0, hi - lo) = arr[lo, hi)
	int lb = mi - lo; int(*B)[2] = new int[lb][2]; //ǰ������B[0, lb) = _elem[lo, mi)
	for (int i = 0; i < lb; (B[i][0] = A[i][0], B[i][1] = A[i++][1])); //����ǰ������
	int lc = hi - mi; int(*C)[2] = arr + mi; //��������C[0, lc) = _elem[mi, hi)
	for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { //B[j]��C[k]�е�С������Aĩβ
		if ((j < lb) && (!(k < lc) || (B[j][0] <= C[k][0]))) (A[i][0] = B[j][0], A[i++][1] = B[j++][1]);
		if ((k < lc) && (!(j < lb) || (C[k][0] < B[j][0]))) (A[i][0] = C[k][0], A[i++][1] = C[k++][1]);
	}
	delete[] B; //�ͷ���ʱ�ռ�B
}

//�鲢����ʱ�临�Ӷ�O(nlogn),�ռ临�Ӷ�ΪO(n)
//����ά�����x����ֵ�����򣬼�xΪ�����key
void mergeSort(int(*arr)[2], int lo, int hi) { //0 <= lo < hi <= size
	assert(0 <= lo);
	//assert(hi <= sizeof(arr)/sizeof(int));
	assert(lo < hi);

	if (hi - lo < 2) return; //��Ԫ��������Ȼ���򣬷���...
	int mi = (lo + hi) >> 1; //���е�Ϊ��
	mergeSort(arr, lo, mi); mergeSort(arr, mi, hi); //�ֱ�����
	merge(arr, lo, mi, hi); //�鲢
}


//�õ����������е�����Ե���Ŀ
long long inverBetween(int(*arr)[2], int lo, int mi, int hi) {
	long long num_inver = 0;

	int(*A)[2] = arr + lo; //�ϲ��������A[0, hi - lo) = arr[lo, hi)
	int lb = mi - lo; int(*B)[2] = new int[lb][2]; //ǰ������B[0, lb) = arr[lo, mi)
	for (int i = 0; i < lb; i++) {
		B[i][0] = A[i][0];
		B[i][1] = A[i][1];
	} //����ǰ������
	int lc = hi - mi; int(*C)[2] = arr + mi; //��������C[0, lc) = arr[mi, hi)
	for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { //B[j]��C[k]�е�С������Aĩβ
		if ((j < lb) && (!(k < lc) || (B[j][1] < C[k][1]))) {
			A[i][0] = B[j][0];
			A[i][1] = B[j][1];
			num_inver += k; //ע�⣺�˴���k������k - 1�� ������k - mi!!!!!!!!!!!!!!!!!!!
			i++;
			j++;
		}
		if ((k < lc) && (!(j < lb) || (C[k][1] < B[j][1]))) {
			A[i][0] = C[k][0];
			A[i][1] = C[k][1];
			i++;
			k++;
		};
	}
	delete[] B; //�ͷ���ʱ�ռ�B

	return num_inver;
}

//�õ���������arr[lo, hi)�е�����Ե���Ŀ
//ʱ�临�Ӷ�O(nlogn),�ռ临�Ӷ�O(nlogn)
long long inverInside(int(*arr)[2], int lo, int hi) {
	if (hi - lo < 2) return 0;
	int mi = (hi + lo) >> 1;
	//�õ�arr[lo, mi)�ڲ���invertion pairs��
	long long num_inver_left = inverInside(arr, lo, mi);
	//�õ�arr[mi, hi)�ڲ���invertion pairs��
	long long num_inver_right = inverInside(arr, mi, hi);
	//inverBetween���Եõ�[lo, mi)��[mi, hi)֮����������
	return inverBetween(arr, lo, mi, hi) + num_inver_left + num_inver_right;
}

//��ʾhouses������Ԫ�ص�ֵ
void display(int(*houses)[2], int num) {
	for (int i = 0; i < num; i++)
	{
		printf("����ֵΪ��%d %d\n", houses[i][0], houses[i][1]);
	}
}

int main()
{
	//����test cases
	//generateTextCases(1000);

	long long num = 0;//����������
	scanf_s("%d", &num);

	int(*houses)[2] = new int[num][2];

	int x, y;//������x��y����
#ifndef _OJ_
	freopen("input.txt", "r", stdin);
#endif
	for (int i = 0; i < num; i++)
	{
		scanf_s("%d %d", &x, &y);
		houses[i][0] = x;
		houses[i][1] = y;
	}

	//�Ը���Lighthouse������xֵ������
	mergeSort(houses, 0, num);//O(nlogn)

	//��ʾhouses������Ԫ�ص�ֵ
	//display(houses, num);

	long long total_pairs = (num * (num - 1)) / 2;
	printf("%ld", total_pairs - inverInside(houses, 0, num));//O(nlogn)

	delete[] houses;
	return 0;
}

