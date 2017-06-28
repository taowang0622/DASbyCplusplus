// scheduler.cpp : �������(Schedule)
//
//#define NDEBUG //����д�������棬������Ч

#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <Windows.h>
using namespace std;
typedef int Rank;

//const int PERMU_MAX = 40320;
const int PERMU_MAX = 4000000;
const char TEST_STR[9] = "abcdefgh";
//char* ALL_PERMU[PERMU_MAX];
char* ALL_PERMU[PERMU_MAX];
//bool TABLE[8];
bool TABLE[26]; //ӳ��a~z��һά���

////����io , �ýṹ��struct���
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


//����Task
struct Task {
	long long prio_num;
	char* task_name;

	Task(long long prio = 0, char* name = NULL) :prio_num(prio), task_name(name) {}
	~Task() {}

	//���رȽ������Զ���prio_numС�����ȼ��ߣ�������������ȫ������ǡ�С���ѡ�
	bool operator < (Task& task) {
		assert(0 < prio_num);
		assert(((long long)1 << 32) > prio_num);
		assert(0 < task.prio_num);
		assert(((long long)1 << 32) > task.prio_num);

		if (prio_num > task.prio_num) return true;
		if (prio_num == task.prio_num) {
			assert(task_name != NULL);
			assert(task.task_name != NULL);
			if (0 < strcmp(task_name, task.task_name)) return true;
		}
		return false;
	}

	//��ǰ�����prio_num����������������ֵ��С��2^32���򷵻�false
	bool doublePrioNum() {
		assert((0 < prio_num) && (((long long)1 << 32) > prio_num));

		return ((prio_num *= 2) >= ((long long)1 << 32)) ? false : true;
	}
};

//Task���Ӧ�Ĳ�����
struct TaskTester {
	Task* ptr_obj;

	void caseBegin() {
		ptr_obj = new Task();
	}
	void caseEnd() {
		delete ptr_obj;
	}

	void operatorLt(long long prio_num_1, long long prio_num_2, char* name_1 = NULL, char* name_2 = NULL) {
		caseBegin(); {
			ptr_obj->prio_num = prio_num_1;
			ptr_obj->task_name = name_1;
			Task task(prio_num_2, name_2);
			bool result = (*ptr_obj) < task;
			assert(result == (prio_num_1 >= prio_num_2));
			printf("%lld��%lld�����ȼ��ͣ�%s\n", prio_num_1, prio_num_2, result ? "true" : "false");
		}caseEnd();
	}

	void doublePrioNum(long long prio) {
		caseBegin(); {
			ptr_obj->prio_num = prio;
			bool valid = ptr_obj->doublePrioNum();
			assert(ptr_obj->prio_num == (2 * prio));
			printf("origin: %lld current: %lld valid: %s\n", prio, ptr_obj->prio_num, valid ? "true" : "false");
		}caseEnd();
	}

};

//���ȼ�����_ʹ����ȫ�����ʵ��
class PQ_ComplHeap {
	friend struct PQ_ComplHeap_Tester; //����������Ϊ��Ԫ��

private:
	int _size;
	Task** _elem;

	/*****************************************************************************************/
	//�����ڵ�i�ͽڵ�j��ֵ
	inline void swap(int i, int j) {
		Task* temp;
		temp = _elem[j];
		_elem[j] = _elem[i];
		_elem[i] = temp;
	}
	//�ж�_elem[i]�Ƿ�Ϸ�
	inline bool inHeap(Rank i) {
		return (-1 < i) && (_size > i);
	}
	//�ڵ�i���Һ��ӵ���
	inline Rank rightChild(Rank i) {
		return (1 + i) << 1;
	}
	//�ڵ�i�����ӵ���
	inline Rank leftChild(Rank i) {
		return 1 + (i << 1);
	}
	//�ж��Ƿ�����Һ���
	inline bool validRC(Rank i) {
		return inHeap((1 + i) << 1);
	}
	//�ж��Ƿ��������
	inline bool validLC(Rank i) {
		return inHeap(1 + (i << 1));
	}
	//�������һ���ڲ��ڵ����
	inline Rank lastInternal() {
		return ((_size - 1) - 1) >> 1;
	}
	//������Ϊi�Ľڵ㼰�亢��(��������)�����ȼ����Ľڵ����
	Rank properParent(Rank i);
	Rank properChild(Rank i);
	/***************************************************************************************************/
	//����															
	Rank percolateDown(Rank i);
	//����-----Floyd�㷨
	void heapify();

public:
	PQ_ComplHeap() {}
	PQ_ComplHeap(Task** tasks, int size);
	//��������
	~PQ_ComplHeap();

	Task* getMax();

	Task* delMax();

	//������ȣ�mΪԤ�������ִ�����г���
	void scheduler(int m);

};

//������Ϊi�Ľڵ㼰�亢��(��������)�����ȼ����Ľڵ����
Rank PQ_ComplHeap::properParent(Rank i) {
	if (validRC(i)) { //���һ�����Һ��ӣ���Ȼ������
		Rank lc = leftChild(i);
		Rank rc = rightChild(i);
		return (*_elem[i] < *_elem[lc]) ? ((*_elem[lc] < *_elem[rc]) ? rc : lc) : ((*_elem[i] < *_elem[rc]) ? rc : i);
	}
	else {
		if (validLC(i)) { //����������Һ��ӣ���������
			Rank lc = leftChild(i);
			return (*_elem[i] < *_elem[lc]) ? lc : i;
		}
		else  //��������������ӣ�Ҳ���Һ���
			return i;
	}
}
Rank PQ_ComplHeap::properChild(Rank i) {
	if (validRC(i)) { //���һ�����Һ��ӣ���Ȼ������
		Rank lc = leftChild(i);
		Rank rc = rightChild(i);
		return (*_elem[lc] < *_elem[rc]) ? rc : lc;
	}
	else {
		if (validLC(i)) { //����������Һ��ӣ���������
			return leftChild(i);
		}
		else  //��������������ӣ�Ҳ���Һ���
			return i;
	}
}
//����
/*Rank PQ_ComplHeap::percolateDown(Rank i) {
	assert(inHeap(i));

	Rank j;
	while (i != (j = properParent(i))) {
		swap(i, j);
		i = j;
	}

	assert(inHeap(i));
	return i;
}*/
//����-----�Ż���,���ٱȽϲ��������͸�ֵ��������
Rank PQ_ComplHeap::percolateDown(Rank i) {
	assert(-1 < i);
	assert(i < _size);

	Task* temp = _elem[i];
	Rank j;

	while (i != (j = properChild(i))) {
		if (*_elem[j] < *temp) {
			_elem[i] = temp;
			return i;
		}
		else {
			_elem[i] = _elem[j];
			i = j;
		}
	}
	_elem[i] = temp;

	assert(-1 < i);
	assert(i < _size);
	return i;
}
//����-----Floyd�㷨
void PQ_ComplHeap::heapify() {
	assert(0 != _size);

	for (Rank i = lastInternal(); inHeap(i); i--)
	{
		percolateDown(i);
	}
}
//���캯��
PQ_ComplHeap::PQ_ComplHeap(Task** tasks, int size) {
	_elem = tasks;
	_size = size;
	heapify();
}
//��������
PQ_ComplHeap::~PQ_ComplHeap() {
	/*for (int i = 0; i < _size; i++)
	{
		delete _elem[i];
	}*/
}
Task* PQ_ComplHeap::getMax() {
	assert(0 != _size);
	return _elem[0];
}
Task* PQ_ComplHeap::delMax() {
	Task* max = getMax();
	if (1 < _size) {
		_elem[0] = _elem[--_size];
		percolateDown(0);
	}
	else _size--;
	return max;
}
//������ȣ�mΪԤ�������ִ�����г���
void PQ_ComplHeap::scheduler(int m) {
	Task** excuted_sequ = new Task*[m];
	int j = 0;
	for (int i = 0; i < m; i++)
	{
		excuted_sequ[j++] = getMax();
		if (getMax()->doublePrioNum()) {
			assert(0 != _size);
			percolateDown(0);
		}
		else {
			delMax();
			if (0 == _size) break;
		}
	}

	/*for (int i = 0; i < j; i++) {
		printf("%s\n", excuted_sequ[i]->task_name);
	}*/
}


//PQ_ComplHeap��Ӧ��Tester
struct PQ_ComplHeap_Tester {
	PQ_ComplHeap* ptr_obj;
	void caseBegin() {
		ptr_obj = new PQ_ComplHeap();
	}
	void caseEnd() {
		delete ptr_obj;
	}

	void heapify(Task** tasks, int size, Task* target) {
		caseBegin(); {
			ptr_obj->_elem = tasks;
			ptr_obj->_size = size;
			ptr_obj->heapify();
			//assert(ptr_obj->getMax() == target);
			printf("���ȼ���ߣ���%lld, %s��\n", (ptr_obj->getMax())->prio_num, (ptr_obj->getMax())->task_name);
		}caseEnd();
	}

	void delMax(Task** tasks, int size, Task* target) {
		caseBegin(); {
			ptr_obj->_elem = tasks;
			ptr_obj->_size = size;
			ptr_obj->heapify();
			ptr_obj->delMax();
			assert(ptr_obj->getMax() == target);
			printf("delMax()֮�����ȼ���ߣ���%lld, %s��\n", (ptr_obj->getMax())->prio_num, (ptr_obj->getMax())->task_name);
		}caseEnd();
	}

};

////ȫ�����ַ�����abcdefgh�������ѽ���浽����ALL_PERMU[]��
//void permu(int rank = 0) {
//	static int ii = 0;
//	static char temp[9]; //��̬�ֲ�����(scope���ֲ� active duration��ֱ���������)����ʱ����
//	if (7 < rank) {
//		char* str = new char[9];
//		int i = 0;
//		for (; i < 8; i++)
//		{
//			str[i] = temp[i];
//		}
//		str[i] = '\0';
//		ALL_PERMU[ii++] = str;
//	};//�ݹ��
//	for (int i = 0; i < 8; i++) {
//		if (!TABLE[i]) {
//			assert(nullptr != temp);
//			temp[rank] = TEST_STR[i];
//			TABLE[i] = true;
//			permu(rank + 1);
//			TABLE[i] = false;
//		}
//	}
//}
//void permuTexter() {
//	for (int i = 0; i < 10; i++)
//	{
//		for (int j = 0; j < 8; j++)
//		{
//			cout << ALL_PERMU[i][j];
//		}
//		cout << endl;
//	}
//}

//��a~z������8���ַ���ɵ��ַ��������ѽ���浽����ALL_PERMU[]��
bool permu(int rank = 0) {
	static int ii = 0;
	static char temp[9]; //"��̬�ֲ�����"(scope���ֲ� active duration��ֱ���������)����ʱ����
	if (7 < rank) { //�ݹ��
		char* str = new char[9];
		int i = 0;
		for (; i < 8; i++)
		{
			str[i] = temp[i];
		}
		str[i] = '\0';
		ALL_PERMU[ii++] = str;
		if (PERMU_MAX <= ii) return false;
		else return true;
	};
	for (int i = 0; i < 26; i++) {
		if (!TABLE[i]) {
			assert(nullptr != temp);
			temp[rank] = 'a' + i;
			TABLE[i] = true;
			if (!permu(rank + 1)) return false;
			TABLE[i] = false;
		}
	}
	return true;
}
void permuTexter() {
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << ALL_PERMU[i][j];
		}
		cout << endl;
	}
}

int main()
{
	//long long x = 1;
	//printf("%lld\n", (x <<= 32));
	//printf("%lld\n", ((long long)(1) << 32));

	///************************************************************************************************/
	//TaskTester task_tester;
	////����Task���<������ġ�������Բ�����
	//task_tester.operatorLt(10, 20);
	//task_tester.operatorLt(30, 20);
	////task_tester.operatorLt(20, ((long long)(1) << 32));
	//task_tester.operatorLt(20, 20, "world", "hello");
	////����Task���doublePrioNum()�����ġ�������Բ�����
	//task_tester.doublePrioNum(100); //�������
	//task_tester.doublePrioNum((long long)(1) << 31); //�߽����
	////task_tester.doublePrioNum((long long)(1) << 32); //�Ƿ����
	////task_tester.doublePrioNum(0); //�Ƿ����
	///*****************************************************************************************/
	//PQ_ComplHeap_Tester PQ_tester;
	////����PQ��heapify()�ĺ�����Բ���
	////�������
	//Task* tasks_1[5] = { new Task(50, "wang"), new Task(40, "tao"), \
	//	new Task(30, "is"), new Task(20, "an"), new Task(10, "idiot") };
	//PQ_tester.heapify(tasks_1, 5, tasks_1[4]);
	////�˻����
	//Task* tasks_2[5] = { new Task(20, "wang"), new Task(20, "tao"), \
	//	new Task(20, "is"), new Task(20, "an"), new Task(20, "idiot") };
	//PQ_tester.heapify(tasks_2, 5, tasks_2[3]);
	////�߽����
	////PQ_tester.heapify(NULL, 0, NULL);
	////����PQ��delmax()�ĺ�����Բ���
	////�������
	//Task* tasks_3[5] = { new Task(50, "wang"), new Task(40, "tao"), \
	//	new Task(30, "is"), new Task(20, "an"), new Task(10, "idiot") };
	//PQ_tester.delMax(tasks_3, 5, tasks_3[3]);
	////�˻����
	//Task* tasks_4[5] = { new Task(20, "wang"), new Task(20, "tao"), \
	//	new Task(20, "is"), new Task(20, "an"), new Task(20, "idiot") };
	//PQ_tester.delMax(tasks_4, 5, tasks_4[4]);
	////�߽����
	////Task* tasks_5[1] = { new Task(10, "wang") };
	////PQ_tester.delMax(tasks_5, 1, NULL);
	///****************************************************************************************************************/
	////printf("%d", strcmp("Tao", "an"));  //ע���Сд����������������������������������������

	permu();
	//permuTexter();

	//Ϊ�˾�ȷ��ʱ��ʹ�õ���CPU��Ƶ�ķ���
	LARGE_INTEGER begin_time;
	LARGE_INTEGER end_time;
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency); //��ȡCPU��Ƶ

	while (1) {
		cout << "�����빤��ģʽ" << endl << "0.����ģʽ\t1.�������\t2.�˳�\t[ ]\b\b";
		int mode; cin >> mode;  //���빤��ģʽ

		if (0 == mode) { //���ѡ���˲���ģʽ�Ļ�
			unsigned int testcase_num;
			cout << "�������������"; cin >> testcase_num;
			unsigned int task_num = INT_MAX;
			while (task_num > PERMU_MAX) { //��Ҫ���ȵ��������������4000000
				cout << "\n������Ҫ���ȵ����������"; cin >> task_num; cout << endl;
			}
			double all_time = 0, max_time = 0, min_time = 1000; //�ֱ��ʾ��������ʱ�䣬�ʱ�䣬���ʱ��
			int* time_table = nullptr;
			while (nullptr == time_table) time_table = new int[10000]; //����ʱ��ͳ������
			for (int i = 0; i < 1000; i++) time_table[i] = 0;//��ʼ��time_table
			int max_table = 0; //���ڼ�¼time_table������Ԫ�أ��Է�����ʾ

			for (int i = 0; i < testcase_num; i++)
			{
				srand(unsigned(clock()));
				int n = task_num, m = rand();
				Task** tasks = new Task*[n];  //delete!
				for (int j = 0; j < n; j++)
				{
					long long prio_num;
					while ((prio_num = rand()) <= 0);
					tasks[j] = new Task(prio_num, ALL_PERMU[j]); //delete!
				}
				QueryPerformanceCounter(&begin_time);
				if (0 == m) return 0;
				PQ_ComplHeap PQ(tasks, n);
				PQ.scheduler(m);
				QueryPerformanceCounter(&end_time);
				all_time += (double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000; //����������ʱ��
				time_table[int((double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000)]++; //����ǰ��ʱֵ����ͳ�Ʊ�
				if ((double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000> max_time)
				{//ˢ���ʱ��ֵ
					max_time = (double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000;
				}
				if ((double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000 < min_time)
				{//ˢ����Сʱ��ֵ
					min_time = (double)(end_time.QuadPart - begin_time.QuadPart) / frequency.QuadPart * 1000;
				}
				//����ѽ���������ʱ��������%���ҵ�ϵͳƫ��
				cout << "�Ѳ���" << i + 1 << "��\tƽ��" << all_time / (i + 1) << "ms\t���ʱ" << max_time << "ms" << endl;
				
				//�ͷŲ���
				for (int i = 0; i < n; i++)
				{
					assert(nullptr != tasks[i]);
					delete tasks[i];
				}
				delete[] tasks;

			}

		}
	}

	/*int n, m;
	scanf("%d %d", &n, &m);
	if (0 == n) return 0;

	Task** tasks = new Task*[n];
	for (Rank i = 0; i < n; i++)
	{
		long long prio_num;
		char* task_name = new char[10];
		scanf("%lld %s", &prio_num, task_name);
		tasks[i] = new Task(prio_num, task_name);
	}

	if (0 == m) return 0;
	PQ_ComplHeap PQ(tasks, n);
	PQ.scheduler(m);*/

	return 0;
}

