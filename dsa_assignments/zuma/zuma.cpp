// zuma.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <cstdio>
#include <cassert>

//����io , �ýṹ��struct���
const int BUFFER_SIZE = 1 << 20;  //��buffer����Ϊ2^20��byte
struct FastIO {
	char inbuf[BUFFER_SIZE];
	char outbuf[BUFFER_SIZE];
	
	FastIO() {  //���캯��
		setvbuf(stdin, inbuf, _IOFBF, BUFFER_SIZE);
		setvbuf(stdout, outbuf, _IOFBF, BUFFER_SIZE);
	}
}io;

//typedef (ListNode*) ListNodePosi;
#define ListNodePosi ListNode*

//ʵ���б�ڵ���
struct ListNode {
	char data;
	ListNodePosi pred;
	ListNodePosi succ;
};

//ʵ���б���
class List {
private:
	int _size;
	ListNodePosi header;//ͷ�ڱ��ڵ��λ��
	ListNodePosi trailer;//β�ڱ��ڵ��λ��
public:
	//���캯��
	List() {
		header = new ListNode;
		trailer = new ListNode;
		header->succ = trailer;
		header->pred = NULL;
		trailer->pred = header;
		trailer->succ = NULL;

		_size = 0;//��ģ_size��ָ����ɼ��Ľڵ�Ĺ�ģ��������header��trailer
	}

	//��������
	~List() {
		delete header, trailer;
	}

	//��������Ϊdata���½ڵ�嵽λ��Ϊposi�Ľڵ�֮ǰ,�������½ڵ��λ��
	//ΪӦ�ԡ��߽����������ֻ����һ��������ʱ��Ĭ��posiΪtrailer��Ҳ����˵���½ڵ�����б�ĩβ
	ListNodePosi insertB(char data, ListNodePosi posi = NULL) {
		if (NULL == posi)posi = trailer;

		ListNodePosi new_node_posi = new ListNode;
		new_node_posi->data = data;
		new_node_posi->succ = posi;
		new_node_posi->pred = posi->pred;

		posi->pred->succ = new_node_posi;
		posi->pred = new_node_posi;

		_size++;

		return new_node_posi;
	}

	//removeλ��Ϊposi�Ľڵ�
	char remove(ListNodePosi posi) {
		char temp = posi->data;
		posi->pred->succ = posi->succ;
		posi->succ->pred = posi->pred;

		_size--;
		delete posi;
		return temp;
	}

	//��ջ��λ�õ�ת��
	ListNodePosi rankToPosi(int rank) {
		//�˻�����Ĵ���
		//assert((0 <= rank)&&(_size >= rank));
		if (rank < 0)
			rank = 0;
		else if (rank > _size)
			rank = _size;

		ListNodePosi posi = header->succ; //first node��λ��
		while (0 < rank--) {
			posi = posi->succ;
		}
		return posi;
	}

	//zuma�����bead������zuma�������beads��ʾ����
	void zuma(ListNodePosi new_posi, char bead) {
		//assert('A' <= bead <= 'Z');

		ListNodePosi posi = new_posi;
		int counter = 0;//������
		//�жϰ��������bead���ڵ�����������ͬ�������м���
		if ((posi != trailer) && (posi->data != bead)) {
			while (((posi = posi->pred) != header) && (posi->data == bead))
				counter++;
		}
		else {
			if (posi != trailer) {
				counter++;
				ListNodePosi backward_posi = posi;
				while ((backward_posi = backward_posi->succ)->data == bead)
					counter++;
			}
			while ((posi = posi->pred)->data == bead)
				counter++;
		}
		//printf("������bead���ڵ�ǰ����ͬ���ӵ�����Ϊ��%d\n", counter + 1); //����

		//�ж���ͬbead���Ƿ��Ѵﵽ�����ĳ̶�,û�ﵽ�Ͳ��룬�ﵽ��remove����Ȼ��ݹ��������������������beads
		if (2 <= counter) {
			for (int i = 0; i < counter; i++)
			{
				remove(posi->succ);
			}
			if ((posi != header) && (posi->succ != trailer)) {
				ListNodePosi succ = posi->succ;
				zuma(succ, remove(posi));
			}
		}
		else
			insertB(bead, new_posi);

	}

	//��ʾ�б���ÿһ��Ԫ�ص�dataֵ
	void display() {
		ListNodePosi posi = header;
		if (posi->succ == trailer)
			printf("-\n");
		else {
			while ((posi = posi->succ) != trailer) {
				printf("%c", posi->data);
			}
			printf("\n");
		}

	}
};



int main()
{
	//setvbuf(stdout, NULL, _IOLBF, 2048);

	List beads;
	char bead;
	while ((scanf_s("%c", &bead), '\n' != bead))
	{
		beads.insertB(bead);
	}

	int num_insert;
	scanf_s("%d", &num_insert);

	int rank;
	char new_bead;
	for (int i = 0; i < num_insert; i++)
	{
		scanf_s("%d %c", &rank, &new_bead);
		//zuma�����bead������zuma�������beads��ʾ����
		ListNodePosi posi = beads.rankToPosi(rank);
		beads.zuma(posi, new_bead);
		beads.display();
	}
	return 0;
}

