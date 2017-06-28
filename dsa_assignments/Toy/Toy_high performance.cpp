//Toy"ħ��" ״̬ת��ͼ+BFS

////#define NDEBUG
#include "stdafx.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

//���峣����ȫ�ֱ���**********************************************************************
struct Node {  //10byte ����һ���ڵ㣬�ýṹ��struct���󶨡�״̬���鼰���Ӧ��״̬����
	unsigned char poses[8];
	short unsigned int pos_to_num; //ע�⣺�˴���������Ϊshort int(��Χ��[0, 32767])����short unsigned int(��Χ[0�� 65535])
	//��pos_to_num�ķ�ΧΪ[0, 40319]������Ϊshort int�Ļ������Ի����
};
const int MAX_TOY = 40320; //8��= 40320��������ߵ����״̬��
const int fac[8] = { 1, 1, 2, 6, 24, 120, 720, 5040 }; //Cantor expansion���õ���0~7�Ľ׳�
unsigned char HASH_TABLE[MAX_TOY]; //39KB ɢ�б�
Node QUEUE[MAX_TOY]; //394KB ���Ѷ���
int ANSWERS[MAX_TOY];  //158KB Ԥ����--������㷨�ľ������ڣ���������������������������������������������


//��������*************************************************************************
int cantor(Node &node) {  //����չ�� ���ݹ�ģ��n = 8 ʱ�临�Ӷȣ�O(n^2) = O(64)������CPUΪ1GHz����ʱ��ɺ��Բ��ƣ���O(1) �ռ临�Ӷȣ��������ռ�ΪO(1)
	int temp, num = 0;
	for (int i = 0; i < 8; i++)
	{
		temp = 0;
		for (int j = i + 1; j < 8; j++)
		{
			if (node.poses[i] > node.poses[j]) temp++;
		}
		num += temp * fac[7 - i];
	}

	return num;
}

void swap(unsigned char& a, unsigned char& b) {
	a = a^b;
	b = b^a;
	a = a^b;
}
void swapTester(unsigned char a, unsigned char b) {
	printf("%d %d => ", a, b);
	swap(a, b);
	printf("%d %d", a, b);
}

void turn(Node &node, int i) {

	if (0 == i)//���·�ת
	{
		swap(node.poses[0], node.poses[7]); swap(node.poses[1], node.poses[6]);
		swap(node.poses[2], node.poses[5]); swap(node.poses[3], node.poses[4]);
	}
	else if (1 == i)//ѭ������
	{
		unsigned char old_position3 = node.poses[3];
		node.poses[3] = node.poses[2]; node.poses[2] = node.poses[1]; node.poses[1] = node.poses[0]; node.poses[0] = old_position3;
		unsigned char old_position4 = node.poses[4];
		node.poses[4] = node.poses[5]; node.poses[5] = node.poses[6]; node.poses[6] = node.poses[7]; node.poses[7] = old_position4;
	}
	else if (2 == i)//����˳ʱ����ת
	{
		unsigned char old_position6 = node.poses[6];
		node.poses[6] = node.poses[5]; node.poses[5] = node.poses[2]; node.poses[2] = node.poses[1]; node.poses[1] = old_position6;
	}
	else assert(false);

}


//��ʼ��**************************************************************************************
void init() {
	memset(HASH_TABLE, 0, MAX_TOY*sizeof(unsigned char));//��ʼ��ɢ�б�HASH_TABLE[]
	int front = 0, rear = 0; //���еĶ�ͷָ��Ͷ�βָ��

	Node node = { { 1, 2, 3, 4, 5, 6, 7, 8 }, 0 }; //������ħ�顱�Ļ�ԭ̬�Ľڵ�node

	QUEUE[front] = node; HASH_TABLE[node.pos_to_num] = 1; //��ʼ��ɢ�б�͹��Ѷ��е���Ԫ��
	Node new_node; //����Ҫ����turn��ħ��ĸ���

	while (front <= rear) {
		Node now = QUEUE[front]; front++; //���ӣ���now��������ģʽ��turn����

		for (int mode = 0; mode < 3; mode++)
		{
			new_node = now; //���ƹ���
			turn(new_node, mode);
			new_node.pos_to_num = cantor(new_node);
			////����
			//for (int i = 0; i < 7; i++)
			//{
			//	printf("%d ", NUM_TO_POS[result][i]);
			//}
			//printf("\n");

			if (HASH_TABLE[new_node.pos_to_num] == 0) {
				ANSWERS[new_node.pos_to_num] = HASH_TABLE[now.pos_to_num]; //��������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				HASH_TABLE[new_node.pos_to_num] = HASH_TABLE[now.pos_to_num] + 1;
				rear++; QUEUE[rear] = new_node;
			}
		}
	}
}



//������************************************************************************************
int main()
{
	init(); //��ʼ��-���

//#define NREDIR
#ifndef NREDIR
	freopen("input.txt", "r", stdin);
#endif


	int num_toy; scanf("%d", &num_toy);
	if (0 == num_toy) return 0;
	unsigned char pos[8];
	unsigned char output[1000];
	for (int i = 0; i < num_toy; i++)
	{
		unsigned char code_table[10];
		
		for (int j = 0; j < 8; j++)
		{
			scanf("%d", pos + j);
		}

		//���������ѧ�ġ��û������ѳ�ʼ״̬�û�Ϊ12345678��Ȼ������ͬ��code_table[]�û�Ŀ��״̬12345678
		//����ʵ�û������ͳ�ʼ״̬��һ���ģ�����Ϊ��ʹ���㷨����"��չ"Ϊָ����ʼ״̬��Ŀ��״̬��Ȼ�����Ĺ��ܣ�����д������ѭ����
		for (int i = 0; i < 8; i++)
		{
			code_table[pos[i]] = i + 1;
		}
		Node node;
		for (int i = 0; i < 8; i++)
		{
			node.poses[i] = code_table[i + 1];
		}

		output[i] = ANSWERS[cantor(node)];
	}

	for (int i = 0; i < num_toy; i++)
	{
		printf("%d\n", output[i]);
	}

	return 0;
}

