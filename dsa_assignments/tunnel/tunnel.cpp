// tunnel.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#define NDEBUG

#include <cstdio>
#include <cassert>
#include <cstdlib>

//����io
//const int BUFFER_SIZE = (1 << 20);
//struct fastIO {
//	char buffer_in[BUFFER_SIZE];
//	char buffer_out[BUFFER_SIZE];
//
//	fastIO() { //���캯��
//		setvbuf(stdout, buffer_out, _IOFBF, BUFFER_SIZE);
//		setvbuf(stdin, buffer_in, _IOFBF, BUFFER_SIZE);
//	}
//} io;

class Stack {
private:
	struct Elem { int data; int counter; }; //����ջ��ÿ��Ԫ������������

	int _size; //ԭջ��ģ����Ҫ����̬���¡�
	int _mirror_size; //�����ģ������Ҫ����̬���¡�
	int _capacity; //����
	int* _elem;
	Elem* _mirror; //��ǰջ�ġ����񡱣��뵱ǰջ��Ԫ��һһ��Ӧ
public:
	Stack(int capacity) {
		_capacity = capacity;
		_size = 0;
		_mirror_size = 0;
		_elem = new int[_capacity];
		_mirror = new Elem[_capacity];
	}
	Stack():Stack(10000){}
	~Stack() {
		delete[] _elem;
		delete[] _mirror;
	}

	//ѹջ�����ջ����ͷ���false
	bool push(int data) {
		if (full())
			return false;
		else {
			//ԭջѹջ
			assert(_size < _capacity);
			_elem[_size] = data;
			++_size; //��̬����ԭջ��_size

					 //����ѹջ
			if (0 == _mirror_size) {
				Elem a_elem = { data, 1 };
				_mirror[_mirror_size] = a_elem;//�߽����
				_mirror_size++;
			}
			else if (data <= _mirror[_mirror_size - 1].data) {
				assert((_size - 1) >= 0); //����
				_mirror[_mirror_size - 1].counter++;
			}
			else {
				Elem a_elem = { data, 1 };
				_mirror[_mirror_size] = a_elem;
				_mirror_size++;
			}

			return true;
		}
	}

	//��ջ�����ص�����Ԫ��,���ջΪ�յ�����µ�ջ���ͷ���-1
	int pop() {
		if (!empty()) {
			if (!(--_mirror[_mirror_size - 1].counter)) {
				_mirror_size--;
			}
			return _elem[--_size];
		}
		else
			return -1;
	}

	//�ж�ջ�Ƿ�Ϊ��
	bool empty() {
		return 0 == _size;
	}

	//�ж�ջ�Ƿ�����
	bool full() {
		return _size == _capacity;
	}

	//����ջ��Ԫ��
	int top() {
		return _elem[_size - 1];
	}

	int size() {
		return _size;
	}

	//��Ч��ȡջ�е����Ԫ�أ�����ʱ�临�Ӷ�ΪO(1)
	//Լ�����������ջ�Ļ�������-1
	int getMax() {
		//assert((_mirror_size - 1) >= 0);
		if (0 == _mirror_size) return -1;
		return _mirror[_mirror_size - 1].data;
	}

	//�ѵ�ǰջ���㵹����ջtarget��
	void dumpTo(Stack& target) {
		int size = _size; //��ǰջ��size���Է�pop��pushʱ_size��̬���£�Ӱ�����ѭ������
		for (int i = 0; i < size; i++)
		{
			target.push(pop());
		}
	}

	//���ڵ���
	void display() {
		printf("top: ");
		for (int i = _size; i > 0; i--)
		{
			printf("%d ", _elem[i - 1]);
		}
		printf(" :bottom\n");

		printf("mirror -> top: ");
		for (int i = _mirror_size; i > 0; i--)
		{
			printf("(data:%d counter:%d) ", _mirror[i - 1].data, _mirror[i - 1].counter);
		}
		printf(" :bottom\n");
	}
};


//������ջ��ʵ��һ������
class Queue {
private:
	Stack* tail;
	Stack* head;
public:
	Queue(int capacity) {
		tail = new Stack(capacity);
		head = new Stack(capacity);
	}
	Queue():Queue(10000){}
	~Queue() {
		delete tail, head;
	}

	void enqueue(int data) {
		assert(!tail->full());//ջtail������
		tail->push(data);
	}

	int dequeue() {
		if (head->empty()) {//���ջhead��Ϊ�գ��Ͱ�tail�е�Ԫ�ء��㵹�������У��ڴ�head�е���
			tail->dumpTo(*head);
			return head->pop();
		}
		else
			return head->pop();
	}

	//��Ч��ȡ���ֵ��O(1)
	//��Ϊ������Stack����Լ�����ˣ����ջΪ�յĻ�������-1
	int getMax() {
		return (tail->getMax() < head->getMax()) ? head->getMax() : tail->getMax();
	}

	//����
	void display() {
		printf("tailջ��\n"); tail->display();
		printf("headջ��\n"); head->display();
	}
};


int main()
{
	//	//���ɲ���
	////#define _REDIR_
	//#ifndef _REDIR_
	//	//freopen("push_test_cases.txt", "w", stdout);
	//	freopen("push_test_cases.txt", "r", stdin);
	//#endif
	//	int num_cases = 50;
	//	int size = 5;
	//	/*for (int i = 0; i < num_cases; i++)
	//	{
	//		for (int j = 0; j < size; j++) {
	//			printf("%d ", rand() % 20);
	//		}
	//		printf("\n");
	//		printf("\n");
	//	}*/
	//
	//	//test
	//	for (int i = 0; i < num_cases; i++)
	//	{
	//		//Stack test;
	//		Queue test(size);
	//		int temp;
	//		/*for (int i = 0; i < size; i++)
	//		{
	//			scanf("%d", &temp);
	//			test.push(temp);
	//		}*/
	//
	//		for (int i = 0; i < size; i++)
	//		{
	//			scanf("%d", &temp);
	//			test.enqueue(temp);
	//		}
	//		printf("��ӣ�\n");
	//		test.display();
	//		printf("�����и�Ԫ�ص����ֵΪ��%d\n", test.getMax());
	//		printf("\n");
	//
	//		printf("���ӣ�\n");
	//		for (int i = 0; i < size; i++)
	//		{
	//			printf("Ԫ��%d����\n", test.dequeue());
	//			test.display();
	//			printf("�����и�Ԫ�ص����ֵΪ��%d\n", test.getMax());
	//			printf("\n");
	//		}
	//
	//		/*printf("ѹջ��\n");
	//		test.display();
	//		printf("ջ�и�Ԫ�ص����ֵΪ��%d\n", test.getMax());
	//		printf("\n");
	//
	//		Stack target;
	//		printf("�㵹��ջtarget�У�\n");
	//		test.dumpTo(target);
	//		target.display();
	//		printf("ջ�и�Ԫ�ص����ֵΪ��%d\n", target.getMax());
	//		printf("\n");
	//
	//		printf("ԭջ��\n");
	//		test.display();*/
	//
	//		/*printf("��ջ��\n");
	//		for (int i = 0; i < size; i++) {
	//			printf("����һ��Ԫ�أ�%d\n", test.pop());
	//			test.display();
	//			printf("ջ�и�Ԫ�ص����ֵΪ��%d\n", test.getMax());
	//			printf("\n");
	//		}*/
	//		printf("****************************************\n");
	//	}

	int num_oper;
	scanf("%d", &num_oper);
	Queue tunnel(num_oper);

	for (int i = 0; i < num_oper; i++)
	{
		char code[10];
		int vehi_height;
		scanf("%s", &code);  //���ɣ����Զ����˿հ׷�(�س����ո�ȣ�
		//if ('\n' == code)scanf("%c", &code);
		switch (code[0])
		{
		case 'E':
			scanf("%d", &vehi_height);
			tunnel.enqueue(vehi_height);
			break;
		case 'D':
			printf("%d\n", tunnel.dequeue());
			break;
		case 'M':
			printf("%d\n", tunnel.getMax());
			break;
		default:
			printf("Invalid Input\n");
			break;
		}
	}

	return 0;
}

