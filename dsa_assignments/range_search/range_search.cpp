// range_search.cpp : �������̨Ӧ�ó������ڵ㡣
//

#define NDEBUG

#include <cassert>
#include <iostream>
#include <cstdio>
using namespace std;

typedef int Rank;
const int DEFAULT_CAPACITY = 5;

//���ܣ���vector��װ�˶�����ķ���
template<typename T>
class Vector {
private:
	int _size;//��ģ
	T* _elem;//������
public:
	//���캯��
	Vector():_size(DEFAULT_CAPACITY){
		_elem = new T[_size];
	}
	Vector(int size) :_size(size){
		_elem = new T[_size];
	}

	//��������
	~Vector() {
		delete[] _elem;
	}

	//����[]�����,���ڶ�Ԫ�صġ����á�
	T& operator[] (Rank i) {
		return _elem[i];
	}
    
	//��õ�ǰ�����Ĺ�ģ
	int size() {
		return _size;
	}

	//��"��������"����[lo, hi)�ж��ֲ���targetԪ�أ����ز�����target�����Ԫ�ص���
	Rank binSearch(T& targrt, int lo, int hi) {
		while (lo < hi) {
			Rank mi = (lo + hi) >> 1;
			(targrt < _elem[mi]) ? hi = mi : lo = mi + 1;
		}
		return --lo;
	}

	void merge(Rank lo, Rank mi, Rank hi) { //���������������[lo, mi)��[mi, hi)
		T* A = _elem + lo; //�ϲ��������A[0, hi - lo) = _elem[lo, hi)
		int lb = mi - lo; T* B = new T[lb]; //ǰ������B[0, lb) = _elem[lo, mi)
		for (Rank i = 0; i < lb; B[i] = A[i++]); //����ǰ������
		int lc = hi - mi; T* C = _elem + mi; //��������C[0, lc) = _elem[mi, hi)
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { //B[j]��C[k]�е�С������Aĩβ
			if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
			if ((k < lc) && (!(j < lb) || (C[k] <  B[j]))) A[i++] = C[k++];
		}
		delete[] B; //�ͷ���ʱ�ռ�B
	} //�鲢��õ���������������[lo, hi)

	void mergeSort(Rank lo, Rank hi) { //0 <= lo < hi <= size
		///*DSA*/printf("\tMERGEsort [%3d, %3d)\n", lo, hi);
		if (hi - lo < 2) return; //��Ԫ��������Ȼ���򣬷���...
		int mi = (lo + hi) / 2; //���е�Ϊ��
		mergeSort(lo, mi); mergeSort(mi, hi); //�ֱ�����
		merge(lo, mi, hi); //�鲢
	}
};


//���ܣ���Χ���Һ�������������src����Ŀ������[a, b]�е�Ԫ�ظ���
int rangeSearch(int a, int b, Vector<int> &src) {
	assert(a <= b);

	src.mergeSort(0, src.size());

	int a_rank = src.binSearch(a, 0, src.size());
	//cout << a_rank << " ";
	int b_rank = src.binSearch(b, 0, src.size());
	//cout << b_rank << " ";

	bool exist = ((a_rank != -1)&&(src[a_rank] == a));
	//cout << exist << " ";

	if (exist) {
		//cout << "num: " << b_rank - a_rank + 1 << endl;
		return b_rank - a_rank + 1;
	}
	else
		//cout << "num: " << b_rank - a_rank << endl;
		return b_rank - a_rank;
}

//int main()
//{
//	//����
//	//Vector<int> vec(5);
//	//int temp;
//	//for (int i = 0; i < vec.size(); i++)
//	//{
//	//	cin >> temp;
//	//	vec[i] = temp;
//	//}
//	//rangeSearch(3, 3, vec);
//	//rangeSearch(4, 4, vec);
//	//rangeSearch(4, 6, vec);
//	//rangeSearch(4, 5, vec);
//	//rangeSearch(5, 5, vec);
//	//rangeSearch(7, 2, vec);
//	//rangeSearch(2, 11, vec);
//
//
//
//	int size = 0, num_query = 0;
//	//cin >> size >> num_query;
//	scanf_s("%d %d", &size, &num_query);
//
//	int temp;
//	Vector<int> points(size);
//	for (int i = 0; i < size; i++)
//	{
//		//cin >> temp;
//		scanf_s("%d", &temp);
//		points[i] = temp;
//	}
//
//	Vector<int> outcomes(num_query);
//	int temp_a, temp_b;
//	for (int i = 0; i < num_query; i++)
//	{
//		//cin >> temp_a >> temp_b;
//		scanf_s("%d %d", &temp_a, &temp_b);
//		outcomes[i] = rangeSearch(temp_a, temp_b, points);
//	}
//
//	for (int i = 0; i < outcomes.size(); i++)
//	{
//		//cout << outcomes[i] << endl;
//		printf_s("%d\n", outcomes[i]);
//	}
//
//	return 0;
//}
//
