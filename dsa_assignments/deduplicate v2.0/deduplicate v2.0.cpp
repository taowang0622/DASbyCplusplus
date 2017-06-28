// deduplicate.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
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

const unsigned SIZE_HASHTABLE = 1200007;
#define hash(hash_code) ((hash_code) % SIZE_HASHTABLE) //ɢ�к��������෨��division method��
//#define NDEBUG

//������
struct Entry {
	int hash_code;
	char* str;
	bool duplicate; //�Ƿ��ظ���־λflag

	Entry(int hash_code, char* str, bool flag = false) :hash_code(hash_code), str(str), duplicate(flag) {}
};


//ɢ�б���
class Hashtable {
private:
	Entry** ht;
	//Entry* ht[SIZE_HASHTABLE];

	//��ͻ�Ž���ԣ�������̽��linear probing��
	//hit���������е��� miss������-1
	int probe4Hit(int hash_code) {
		int rank = hash(hash_code);
		while (ht[rank] && hash_code != ht[rank]->hash_code) { //ע�⣺���߼����ʽ�����õ�ǰ����Ҫ��ÿ��Ͱ��Ԫ��ʼ��
			rank = (rank + 1) % SIZE_HASHTABLE;
		}
		return ht[rank] ? rank : -1;
	}

	int probe4Free(int hash_code) {
		int rank = hash(hash_code);
		while (ht[rank]) rank = (rank + 1) % SIZE_HASHTABLE;
		return rank; //Ϊ��֤�����ҵ���Ͱ��װ�����Ӻ�hash table������Ҫ��������
	}

public:
	Hashtable() {
		ht = new Entry*[SIZE_HASHTABLE];
		memset(ht, 0, sizeof(Entry*) * SIZE_HASHTABLE); //��ʼ��ÿ����Ͱ��Ԫ��
	}
	//~Hashtable(){}

	//�����ַ�����ѭ����λɢ����cyclic shift hash code
	//����ΪC�����ַ������ַ����飩
	int hashCode(char* str) {
		int h = 0;
		for (int length = strlen(str), i = 0; i < length; i++)
		{
			h = (h << 5) | (h >> 27);
			h += (int)str[i];
		}
		return h;
	}

	//����ؼ�������Ϊchar*(C����ַ���)�Ĵ���
	bool put(char *str) {
		int hash_code = hashCode(str);
		int rank;
		if ((rank = probe4Hit(hash_code)) != -1) {
			if (strcmp(ht[rank]->str, str) == 0) {
				if (false == (ht[rank]->duplicate)) {
					ht[rank]->duplicate = true; printf("%s\n", str); return false;
					//��һ�γ����ظ����ͰѶ�Ӧ��Ͱ��Ԫ���ظ���־λ����Ϊtrue������������false
				}
				return true; //��һ���ظ�֮����ظ���ֱ�ӷ���true
			}
		}

		rank = probe4Free(hash_code);
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
	//char** outputs = new char*[num_cases]; //�����洢duplicate���ַ��������ַ��ġ�ָ�롱
	//int rank_outputs = 0; //����ouputs���ȣ�����cursor������
	for (int i = 0; i < num_cases; i++)
	{
		char* str = new char[45];
		scanf("%s", str);
		hash_table.put(str);
		//puts(temp); //puts()����ַ�������Զ�����
		//printf("    whether succeed in putting: %s\n", hash_table.put(temp) ? "true" : "false");
		/*if (!hash_table.put(str)) {
			outputs[rank_outputs] = str;
			rank_outputs++;*/
		//}
	}

	/*for (int i = 0; i < rank_outputs; i++)
	{
		printf("%s\n", outputs[i]);
	}*/

	return 0;
}

