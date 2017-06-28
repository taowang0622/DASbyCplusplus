#include <cstdio>
#include <cassert>

//��·�鲢��ʱ�临�Ӷ�O(n),�ռ临�Ӷ�O(mi - lo)
void merge(int arr[], int lo, int mi, int hi) {
	int* A = arr + lo; //�ϲ��������A[0, hi - lo) = arr[lo, hi)
	int lb = mi - lo; int* B = new int[lb]; //ǰ������B[0, lb) = _elem[lo, mi)
	for (int i = 0; i < lb; B[i] = A[i++]); //����ǰ������
	int lc = hi - mi; int* C = arr + mi; //��������C[0, lc) = _elem[mi, hi)
	for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { //B[j]��C[k]�е�С������Aĩβ
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] <  B[j]))) A[i++] = C[k++];
	}
	delete[] B; //�ͷ���ʱ�ռ�B
}

//�鲢����ʱ�临�Ӷ�O(nlogn),�ռ临�Ӷ�ΪO(n)
void mergeSort(int arr[], int lo, int hi) { //0 <= lo < hi <= size
	assert(0 <= lo);
	//assert(hi <= sizeof(arr)/sizeof(int));
	assert(lo < hi);

	if (hi - lo < 2) return; //��Ԫ��������Ȼ���򣬷���...
	int mi = (lo + hi) >> 1; //���е�Ϊ��
	mergeSort(arr, lo, mi); mergeSort(arr, mi, hi); //�ֱ�����
	merge(arr, lo, mi, hi); //�鲢
}

//��arr[lo, hi)�ж��ֲ���target
//ʱ�临�Ӷ�ΪO(logn),�ռ临�Ӷ�ΪO(1)
int binSearch(int arr[], int target, int lo, int hi) {
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		(target < arr[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}

//������arr�в�������[a, b]�е�Ԫ�ظ���
int rangeSearch(int arr[], int arr_size, int a, int b) {
	int a_rank = binSearch(arr, a, 0, arr_size);
	int b_rank = binSearch(arr, b, 0, arr_size);

	bool exist = ((a_rank != -1) && (arr[a_rank] == a));
	if (exist) {
		return b_rank - a_rank + 1;
	}
	else
		return b_rank - a_rank;
}

int main() {
	//����points��size��query�Ĵ���
	int size, num_query;
	scanf_s("%d %d", &size, &num_query);

	//������ģΪsize���飬�������������point������
	int* points = new int[size]; //��Ϊ�����size���û�����ģ�����ʹ�ö�̬���飬������delete[]��������������������
	for (int i = 0; i < size; i++) {
		scanf_s("%d", &points[i]);
	}

	//������points���й鲢����
	mergeSort(points, 0, size);

	//��������Ҫ���ҵ�����[a, b],��ʹ��rangeSearch�������ҳ�points����������ڵ�Ԫ�ظ���
	int a, b;
	for (int i = 0; i < num_query; i++)
	{
		scanf_s("%d %d", &a, &b);

		//�˻����
		if (a > b) {
			int temp;
			temp = b;
			b = a;
			a = temp;
		}

		printf("%d\n", rangeSearch(points, size, a, b));
	}

	delete[] points;
	return 0;
}