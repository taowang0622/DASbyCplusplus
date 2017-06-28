// deduplicate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cmath>
//
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

const unsigned SIZE_HASHTABLE = 1200109;
//#define hash(hash_code) ((hash_code) % SIZE_HASHTABLE)
//ɢ�к��������෨��division method������������ȱ��
#define hash(hash_code) ((31 * (hash_code) + 2) % SIZE_HASHTABLE) 
//ɢ�к�����MAD����a��bȡֵǡ���Ļ����Կ˷����෨��������ȱ��
//#define NDEBUG

//������
struct Entry {
	int hash_code;
	char* str;
	bool duplicate; //�Ƿ��ظ���־λflag

	//�ظ���ʶλduplicateĬ��Ϊfalse
	Entry(int hash_code, char* str, bool flag = false) :hash_code(hash_code), str(str), duplicate(flag) {}
};


//ɢ�б���
class Hashtable {
private:
	Entry** ht;
	//Entry* ht[SIZE_HASHTABLE];  //����new֮�⣬������ͺ������ڶ���Ķ��󣬶�����ʱ�ģ��洢��ջ�У��Թ�ģ������

	//�����ַ�����ѭ����λɢ����cyclic shift hash code
	//����ΪC�����ַ������ַ����飩
	int hashCode(const char* str) {
		int h = 0;
		for (int length = strlen(str), i = 0; i < length; i++)
		{
			h = (h << 5) | (h >> 27);
			h += (int)str[i];
		}
		return h;
	}

	//����ʽɢ����polynomial hash code
	/*int hashCode(const char* str) {
		int temp = 33;
		int code = 0;
		for (int length = strlen(str), i = 0; i < length; i++)
		{
			code += int(str[i]) * pow(temp, length - i - 1);
		}
		return code;
	}*/

	//��ͻ�Ž���ԣ�ƽ����̽��quadratic probing��
	//����Ϊ�ؼ��루�ַ�����
	int probe4Hit(const char* str) {
		int hash_code = hashCode(str);
		int rank = hash(hash_code);
		int i = 0;
		//ע�⣺���߼����ʽ�����õ�ǰ����Ҫ��ÿ��Ͱ��Ԫ��ʼ��,C++���߼����ʽ�Ǻö�������������������������
		while (ht[rank] && (hash_code != ht[rank]->hash_code || (strcmp(str, ht[rank]->str) != 0))) {
			rank = (rank + i * i) % SIZE_HASHTABLE;
			i++;
		}
		return ht[rank] ? rank : -1;
	}

	//��ͻ�Ž���ԣ�ƽ����̽��quadratic probing��
	int probe4Free(const char* str) {
		int rank = hash(hashCode(str));
		int i = 0;
		while (ht[rank]) {
			rank = (rank + i * i) % SIZE_HASHTABLE; i++;
		}
		return rank; //Ϊ��֤�����ҵ���Ͱ��װ�����Ӻ�hash table������Ҫ��������
	}

public:
	Hashtable() {
		ht = new Entry*[SIZE_HASHTABLE];
		memset(ht, 0, sizeof(Entry*) * SIZE_HASHTABLE); //��ʼ��ÿ����Ͱ��Ԫ��
	}
	//��������
	~Hashtable() {
		for (int i = 0; i < SIZE_HASHTABLE; i++)
		{
			if (!ht[i]) continue;
			delete[] ht[i]->str;
			delete ht[i];
		}
		delete[] ht;
	}

	//����ؼ�������Ϊchar*(C����ַ���)�Ĵ���
	bool put(char *str) {
		int hash_code = hashCode(str);
		int rank;
		if ((rank = probe4Hit(str)) != -1) {
			if (false == (ht[rank]->duplicate)) {
				ht[rank]->duplicate = true; return false;
				//��һ�γ����ظ����ͰѶ�Ӧ��Ͱ��Ԫ���ظ���־λ����Ϊtrue������������false
			}
			return true; //��һ���ظ�֮����ظ���ֱ�ӷ���true
		}

		rank = probe4Free(str);
		//Entry new_entry = { hash_code, str };
		ht[rank] = new Entry(hash_code, str);
		return true;
	}

};



int main()
{
	//#define NREDIR
#ifndef NREDIR
	freopen("test_cases_1.txt", "r", stdin);
#endif

	Hashtable hash_table;
	int num_cases;
	scanf("%d\n", &num_cases);
	char** outputs = new char*[num_cases]; //�����洢duplicate���ַ��������ַ��ġ�ָ�롱
	int rank_outputs = 0; //����ouputs���ȣ�����cursor������
	for (int i = 0; i < num_cases; i++)
	{
		char* str = new char[45];
		scanf("%s", str);
		//puts(temp); //puts()����ַ�������Զ�����
		//printf("    whether succeed in putting: %s\n", hash_table.put(temp) ? "true" : "false");
		if (!hash_table.put(str)) {
			outputs[rank_outputs] = str;
			rank_outputs++;
		}
	}

	for (int i = 0; i < rank_outputs; i++)
	{
		printf("%s\n", outputs[i]);
	}

	return 0;
}

