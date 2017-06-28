// Toy: ״̬ת��ͼ��������
//
//#define NDEBUG

#include "stdafx.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

//���峣����ȫ�ֱ���**********************************************************************
const int MAX_TOY = 40320; //8��= 40320��������ߵ����״̬��
int POS_TO_NUM[8][8][8][8][8][8][8]; //8MB �ռ�������2%  pos_to_num[x1][x2][x3][x4][x5][x6][x7]��ʾ����x1x2x3x4x5x6x7��ȫ����ǰ��λ�е�λ��
unsigned char NUM_TO_POS[MAX_TOY][7]; //275KB
unsigned char HASH_TABLE[MAX_TOY]; //39KB
int QUEUE[MAX_TOY]; //158KB

//��ʼ������*********************************************************************

//���POS_TO_NUM[]�� NUM_TO_POS[][]: ˳��ö�ٳ�0~7��ȫ���е�ǰ��λ������¼���������С�
//i��ʾö�����еĵ�iλ��rank��ʾ��ǰ������ȫ�����е�λ��
//������ģ:40320 ʱ�临�Ӷ�:O(40320)����cpu��1GHz��������Ҫ0.04ms �ռ临�Ӷ�:19byte���� 
void init(int i) {
	assert(0 <= i);
	assert(7 >= i);
	static int rank = 0;
	static bool check_num[8]; //�ñ�����ڼ��i��״̬check_num[i],��Ϊfalse����ʾ�����ں��漸λ��ö�٣���Ϊtrue�����ʾ��ռ��
	static unsigned char permu[7];//static�ֲ���������Ϊ��ʼ������Ĭ�ϳ�ʼ��Ϊ0
	
	if (i == 7) {//�ݹ�������ݹ�ֽ�ı߽�
		for (int j = 0; j < 7; j++) NUM_TO_POS[rank][j] = permu[j];
		POS_TO_NUM[permu[0]][permu[1]][permu[2]][permu[3]][permu[4]][permu[5]][permu[6]] = rank;
		rank++; //λ�����1
		return;
	}
	for (int num = 0; num < 8; num++)
	{
		if (false == check_num[num]) {
			check_num[num] = true;  //��Ҫ
			permu[i] = num;
			init(i + 1); //�ݹ飨�ݹ�ֽ�+�𼶻ع飩
			check_num[num] = false; //�����ˣ����������� 
		}
	}

}
void preTester(int num_case) {
	init(0);
	for (int i = 0; i < num_case; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			printf("%d ", NUM_TO_POS[i][j]);
		}
		printf("\n");
	}
}

//��������*************************************************************************
//������int��������ʵ�ֽ���
void swap(int& a, int& b) {
	a = a^b;
	b = b^a;
	a = a^b;
}
void swapTester(int a, int b) {
	printf("%d %d => ", a, b);
	swap(a, b);
	printf("%d %d", a, b);
}

//��״̬����num_pos����ת�����Ϊi��ת��
int turn(int num_pos, int i) {
	assert(0 <= num_pos);
	assert(MAX_TOY > num_pos);

	//��ѹ��unzip����״̬���ֽ�ѹ��Ϊ״̬����
	int* positions = new int[8];
	int adder = 0; //ͳ������ǰ7λ���ܺ�
	for (size_t j = 0; j < 7; j++)
	{
		adder += NUM_TO_POS[num_pos][j];
		positions[j] = NUM_TO_POS[num_pos][j];
	}
	positions[7] = 28 - adder; //���һλ�ڴ��ʱû�д洢�����Խ�ѹ��ʱҪ���⸳ֵ

	if (0 == i)//���·�ת
	{
		swap(positions[0], positions[7]); swap(positions[1], positions[6]);
		swap(positions[2], positions[5]); swap(positions[3], positions[4]);
	}
	else if (1 == i)//ѭ������
	{
		int old_position3 = positions[3];
		positions[3] = positions[2]; positions[2] = positions[1]; positions[1] = positions[0]; positions[0] = old_position3;
		int old_position4 = positions[4];
		positions[4] = positions[5]; positions[5] = positions[6]; positions[6] = positions[7]; positions[7] = old_position4;
	}
	else if (2 == i)//����˳ʱ����ת
	{
		int old_position6 = positions[6];
		positions[6] = positions[5]; positions[5] = positions[2]; positions[2] = positions[1]; positions[1] = old_position6;
	}
	else assert(false);

	int new_num_pos = POS_TO_NUM[positions[0]][positions[1]][positions[2]][positions[3]][positions[4]][positions[5]][positions[6]];
	delete[] positions;
	return new_num_pos;
}

//����״̬����Ϊnum_pos��Toy�������ص���ԭ̬�����·��
int search(int num_pos) {
	assert(0 <= num_pos);
	assert(MAX_TOY > num_pos);
	memset(HASH_TABLE, 0, MAX_TOY*sizeof(unsigned char));//��ʼ��ɢ�б�HASH_TABLE[]
	int front = 0, rear = 0; //���У�ÿ��Ԫ����״̬����

	if (0 == num_pos) return 0; //��ʾToy�Ѿ���ԭ��
	QUEUE[front] = num_pos; HASH_TABLE[num_pos] = 1; //��ʼ��ɢ�б�͹��Ѷ��е���Ԫ��

	while (front <= rear) {
		int now = QUEUE[front]; front++;

		for (int mode = 0; mode < 3; mode++)
		{
			int result = turn(now, mode);

			////����
			//for (int i = 0; i < 7; i++)
			//{
			//	printf("%d ", NUM_TO_POS[result][i]);
			//}
			//printf("\n");

			/*int result_rank = POS_TO_NUM[result[0]][result[1]][result[2]][result[3]][result[4]][result[5]][result[6]];*/

			if (0 == result) return HASH_TABLE[now];

			if (HASH_TABLE[result] == 0) {
				HASH_TABLE[result] = HASH_TABLE[now] + 1;
				rear++; QUEUE[rear] = result;
			}
		}
	}
	return -1;
}

//������************************************************************************************
int main()
{
	init(0); //��ʼ��-���

#define NREDIR
#ifndef NREDIR
	freopen("input.txt", "r", stdin);
#endif


	int num_toy; scanf("%d", &num_toy);
	if (0 == num_toy) return 0;
	unsigned char pos[8];
	unsigned char output[1000];
	for (int i = 0; i < num_toy; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			scanf("%d", pos + j);
		}

		int num_pos = POS_TO_NUM[pos[0] - 1][pos[1] - 1][pos[2] - 1][pos[3] - 1][pos[4] - 1][pos[5] - 1][pos[6] - 1];

		output[i] = search(num_pos);
	}

	for (int i = 0; i < num_toy; i++)
	{
		printf("%d\n", output[i]);
	}

	return 0;
}

//���㷨low performance��ԭ���ǣ���input�ܶ����ħ�顱ʱ���ᷴ������search()��ÿһ��BFS�Ĺ����бض����в�����ǰ����BFS�ظ�������û��Ҫ���ظ���������Ч��