// Train Dispatching.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define NDEBUG

#include<cstdio>
#include<cassert>
#include <cstdlib>

//����io , �ýṹ��struct���
//const int BUFFER_SIZE = 1 << 20;  //��buffer����Ϊ2^20��byte
//struct FastIO {
//	char inbuf[BUFFER_SIZE];
//	char outbuf[BUFFER_SIZE];
//
//	FastIO() {  //���캯��
//		setvbuf(stdin, inbuf, _IOFBF, BUFFER_SIZE);
//		setvbuf(stdout, outbuf, _IOFBF, BUFFER_SIZE);
//	}
//}io;

enum Operation { PUSH, POP };

class Node {
public:
	int _data;
	Node* _pre;
	Node* _succ;

	Node() {}
	Node(int data) :_data(data), _pre(NULL), _succ(NULL) {}
};

class Queue {
private:
	Node *_header, *_trailer;
public:
	Queue() {
		_header = new Node();
		_trailer = new Node();
		_header->_succ = _trailer;
		_trailer->_pre = _header;
	}
	~Queue() {
		delete _header, _trailer;
	}

	//�Ӷ�β���,Ȼ�󷵻�����ӵ�Ԫ�ص�λ��
	Node* enqueue(int data) {
		Node* new_node = new Node(data);
		new_node->_succ = _trailer;
		new_node->_pre = _trailer->_pre;
		_trailer->_pre->_succ = new_node;
		_trailer->_pre = new_node;

		return new_node;
	}

	//�������,�Ӷ�ͷ���
	Node* reverEnqueue(int data) {
		Node* new_node = new Node(data);
		new_node->_succ = _header->_succ;
		new_node->_pre = _header;
		_header->_succ->_pre = new_node;
		_header->_succ = new_node;

		return new_node;

	}

	//�Ӷ�ͷ���ӣ�Ȼ�󷵻س��ӵ�Ԫ�ص��������ֵ
	int dequeue() {
		int front_data = _header->_succ->_data;
		Node* front_posi = _header->_succ;

		_header->_succ->_succ->_pre = _header;
		_header->_succ = _header->_succ->_succ;

		delete front_posi;
		return front_data;
	}

	//���ض�ͷ��λ��,�������Ϊ�գ��򷵻�NULL
	Node* front() {
		return (_header->_succ == _trailer) ? NULL : _header->_succ;
	}

	//��front��rear��������ʾ�����ڵ����������ڵ���
	void display() {
		printf("front: ");
		for (Node* posi = _header->_succ; posi != _trailer; posi = posi->_succ)
		{
			printf("%d ", posi->_data);
		}
		printf(" :rear\n");
	}
};

template<typename T>
class Stack {
private:
	int _size; //��ģ����Ҫ����̬���¡�
	int _capacity; //����
	T* _elem;
public:
	Stack(int capacity) {
		_capacity = capacity;
		_size = 0;
		_elem = new T[_capacity];
	}
	Stack() :Stack(1000) {}
	~Stack() {
		delete[] _elem;
	}

	//ѹջ�����ջ����ͷ���false
	bool push(T data) {
		if (_size >= _capacity)
			return false;
		else {
			_elem[_size] = data;
			++_size;
			return true;
		}
	}

	//��ջ�����ص�����Ԫ��
	T pop() {
		return _elem[--_size];
	}

	//�ж�ջ�Ƿ�Ϊ��
	bool empty() {
		return 0 == _size;
	}

	//�ж�ջ�Ƿ�����
	bool full() {
		return _size == _capacity;
	}

	T top() {
		return _elem[_size - 1];
	}

	int size() {
		return _size;
	}

	//���ڵ���
	void display() {
		printf("top: ");
		for (int i = _size; i > 0; i--)
		{
			printf("%d ", _elem[i - 1]);
		}
		printf(" :bottom\n");
	}
};

void remove(Stack<int>& transfer_end, int& ordinal, Stack<Operation>& operations, Queue& entrance) {
	for (int i = transfer_end.size(); i > 0; i--)
	{
		if (ordinal == transfer_end.top()) {
			int temp = transfer_end.pop();
			operations.push(POP);
			ordinal--;
			entrance.reverEnqueue(temp); //���ڵ���
		}
		else
			break;
	}
}

bool feasible(Queue& exit, Queue& entrance, Stack<int>& transfer_end, int num_compart, Stack<Operation>& operations) {
	if (transfer_end.full()) return false; //�˻����
	
	int ordinal = num_compart;//�����������Ϊ���������
	Node* cursor = exit.front();

	//while (num_compart--) {//�Ӷ�ͷ����β����һ������exit����
	//	if (ordinal == cursor->_data) {
	//		if (transfer_end.full()) return false;
	//		entrance.reverEnqueue(ordinal); //�ҵ�ordinal����entrance����
	//										//entrance.display();
	//		operations.push(PUSH); operations.push(POP);
	//		--ordinal;//search��һ������
	//		remove(transfer_end, ordinal, operations, entrance);
	//	}
	//	else {
	//		//transfer_end.display();
	//		if (!transfer_end.empty()) {
	//			remove(transfer_end, ordinal, operations, entrance);
	//		}
	//		if (!transfer_end.empty()) {  //���ջ������ջ����Ϊ�յĻ�
	//			if (cursor->_data < transfer_end.top())//���ѹջ֮ǰ��Ҫѹջ��Ԫ�ر�ջ��Ԫ��С�Ļ����϶�������
	//				return false;
	//		}
	//		if (!(operations.push(PUSH), transfer_end.push(cursor->_data))) {//���ջ�����������No����������
	//			printf("ջ���\n");
	//			return false;
	//		}
	//	}
	//	cursor = cursor->_succ;//cursor��rear��һ��
	//}

	for (int i = 0; i < num_compart; i++) {//�Ӷ�ͷ����β����һ������exit����
		if (ordinal == cursor->_data) {
			if (transfer_end.full()) return false;
			entrance.reverEnqueue(ordinal); //�ҵ�ordinal����entrance����
			//entrance.display();
			operations.push(PUSH); operations.push(POP);
			--ordinal;//search��һ������
			remove(transfer_end, ordinal, operations, entrance);
		}
		else {
			//transfer_end.display();
			if (!transfer_end.empty()) {
				remove(transfer_end, ordinal, operations, entrance);
			}
			if (!transfer_end.empty()) {  //���ջ������ջ����Ϊ�յĻ�
				if (cursor->_data < transfer_end.top())//���ѹջ֮ǰ��Ҫѹջ��Ԫ�ر�ջ��Ԫ��С�Ļ����϶�������
					return false;
			}
			if (!(operations.push(PUSH), transfer_end.push(cursor->_data))) {//���ջ�����������No����������
				printf("ջ���\n");
				return false;
			}
		}
		cursor = cursor->_succ;//cursor��rear��һ��
	}

	//printf("ordinal: %d\n", ordinal); ���ڵ���
	//transfer_end.display();
	while (ordinal) {//��exit������֮�󣬵õ���ջӦ�á��Զ����¡��ʵݼ�����������ǣ��Ͳ�����
		if ((operations.push(POP), ordinal != transfer_end.pop())) {
			return false;
		}
		else {
			entrance.reverEnqueue(ordinal);
		}
		ordinal--;
	}

	//entrance.display();
	return true;
}

//������
void swap(int& a, int& b) {
	int temp;
	temp = a;
	a = b;
	b = temp;
}

//����������
void permute(int arr[], int size) {
	for (int i = size; i > 0; i--) //�Ժ���ǰ
	{
		swap(arr[i - 1], arr[rand() % i]);
	}
}


int main()
{


	//���ɲ���
#define _REDIR_
#ifndef _REDIR_
	//freopen("test_cases.txt", "w", stdout);
	freopen("test_cases.txt", "r", stdin);
#endif
	int n = 5;
	/*int* cases = new int[n];
	for (int i = 0; i < n; i++) {
	cases[i] = i + 1;
	}
	for (int i = 0; i < 5*n; i++) {
	printf("%d ", n);
	printf("%d\n", (rand() % n) + 1);
	permute(cases, n);
	for (int i = 0; i < n; i++)
	{
	printf("%d ", cases[i]);
	}
	printf("\n");
	printf("\n");

	}*/

	/*for (int i = 0; i < 5*n; i++)
	{*/
	int num_compart, limit_tansfer;
	scanf("%d %d", &num_compart, &limit_tansfer);

	Queue exit, entrance;
	Stack<int> transfer_end(limit_tansfer);
	Stack<Operation> operations(2 * num_compart);
	int ordinal;//��������

				//���������exit�г��������
	for (int i = 0; i < num_compart; i++)
	{
		scanf("%d", &ordinal);
		exit.reverEnqueue(ordinal); //����������Ԫ����Ϊfront����һ���������Ϊrear
	}

	if (feasible(exit, entrance, transfer_end, num_compart, operations)) {
		int size = operations.size();
		for (int i = 0; i < size; i++)
		{
			switch (operations.pop())
			{
			case PUSH:printf("pop\n"); break;
			case POP:printf("push\n"); break;
			}
		}
	}
	else
		printf("No");

	//����
	/*printf("\n");
	transfer_end.display();
	entrance.display();
	printf("\n");*/
	/*}*/
	return 0;
}

