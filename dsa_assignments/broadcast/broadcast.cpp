//ʹ��BFS�������������ߵ粨������

#include <cstdio>
#include <cstdlib>
#include <cassert>
//#define NDEBUG

enum VStatus { UNDISCOVERED, DISCOVERED, VISITED };  
//VISITED��ʾ�����ʽ���������ʾ�ýڵ㲻�ܱ��ٴη��ʣ��Ǻ���Ҫ��һ��״̬������û��

//������
struct Vertex {
	VStatus status; //�ڵ�״̬
	int wave_band; //��ǰ�ڵ�Ĳ���

	Vertex(VStatus status = UNDISCOVERED, int spectrum = -1) :status(status), wave_band(wave_band) {}
};

//�߽ڵ���
struct EdgeNode {
	int data;
	EdgeNode* pred;
	EdgeNode* succ;

	EdgeNode(int data = -1, EdgeNode* pred = NULL, EdgeNode* succ = NULL) :data(data), pred(pred), succ(succ) {}
};

//���б��ࣨ�ڽӱ�
class EdgeList {
private:
	int _size;
	EdgeNode(*_header), (*_trailer);
	EdgeNode* _cursor; //�α�
protected:
	void init() {
		_size = 0;
		_header = new EdgeNode();
		_trailer = new EdgeNode();
		_header->succ = _trailer;
		_trailer->pred = _header;
		_cursor = _header; //��cursorָ��ͷ�ڱ�
	}
public:
	EdgeList() { init(); }
	~EdgeList() {
		for (EdgeNode* v = _header->succ; v != _trailer;) {
			_header->succ->succ->pred = _header;
			_header->succ = _header->succ->succ;
			delete v;
			v = _header->succ;
		}
		delete _header;
		delete _trailer;
	}

	void insertAsLast(int data) {
		EdgeNode* newNode = new EdgeNode(data, NULL, _trailer);
		newNode->pred = _trailer->pred;
		_trailer->pred->succ = newNode;
		_trailer->pred = newNode;
		_size++; //������ά������_size
	}
	int removeFirst() {
		assert(0 != _size); //����
		_size--;
		EdgeNode* v = _header->succ;
		_header->succ->succ->pred = _header;
		_header->succ = _header->succ->succ;
		int temp = v->data;
		delete v;
		return temp;
	}

	int size() {
		return _size;
	}

	//�ɵ�һ���ڵ㿪ʼ���������б�,��һ���ڵ��position������_cursor��position����
	int nextNode() {
		if ((0 == _size) || (_cursor == _trailer)) return -1;
		return ((_cursor = _cursor->succ) == _trailer) ? -1 : (*_cursor).data;
	}

	//debug
	void display() {
		if (0 == _size) {
			printf("-1\n");
			return;
		}
		for (EdgeNode* v = _header->succ; v != _trailer; v = v->succ) {
			printf("%d ", v->data);
		}
		printf("\n");
	}

};

//������
class Queue : public EdgeList {
public:
	Queue() {}
	~Queue() {}
	void enqueue(int data) {
		insertAsLast(data);
	}
	int dequeue() {
		assert(0 != size()); //����
		return removeFirst();
	}
	bool empty() {
		return 0 == size();
	}

};

//ʹ��adjacency listʵ��graph
struct GraphList {
	int n;// �������Ŀ
	int e;//�ߵ���Ŀ
	Vertex* vertexes;
	EdgeList** edges; //ʹ���ڽӱ�

					  //n�Ƕ�����Ŀ��m�Ǳߵ���Ŀ
	GraphList(int n, int m) :n(n), e(m) {
		vertexes = new Vertex[n];
		edges = new EdgeList*[n];
		//���������Ԫ�ض���ʼ��Ϊ0
		for (int i = 0; i < n; i++)
		{
			edges[i] = new EdgeList();
		}
	}
	~GraphList() {
		delete[] vertexes;
		for (int i = 0; i < n; i++)
		{
			delete edges[i];
		}
	}

	//�����i��j�ı�
	void insert(int i, int j) {
		(*edges[i]).insertAsLast(j);
	}

	//��ö���i����һ���ھ�
	int nextNbr(int i) {
		return (*edges[i]).nextNode();
	}

	//debug
	void displayEdges() {
		for (int i = 0; i < n; i++)
		{
			(*edges[i]).display();
		}
	}

	//����BFS��ܽ�����ߵ粨���⣨ȫͼ��
	int bfs() {
		int s = rand() % n;
		int v = s;
		do
		{
			if (UNDISCOVERED == vertexes[v].status) {
				if (!BFS(v)) return -1;
			}
		} while (s != (v = (++v % n)));
		return 1;
	}

	//����ͨ��
	//s��ʾ��ʼ����
	bool BFS(int s) {
		Queue Q;
		vertexes[s].status = DISCOVERED; //�ڵ�s�������֡�
		vertexes[s].wave_band = 0; //���ڵ�s�ġ����Ρ���Ϊ0
		Q.enqueue(s);
		while (!Q.empty()) {
			int v = Q.dequeue();
			for (int u = nextNbr(v); -1 < u; u = nextNbr(v))
			{
				switch (vertexes[u].status)
				{
				case UNDISCOVERED:
					vertexes[u].status = DISCOVERED;
					vertexes[u].wave_band = (vertexes[v].wave_band == 0) ? 1 : 0;
					Q.enqueue(u); break;
				case DISCOVERED:
					if (vertexes[u].wave_band == vertexes[v].wave_band) return false; break;
				case VISITED:
					break;
				}
			}
			vertexes[v].status = VISITED;
		}
		return true;
	}
};



int main()
{
	/*Queue queu;
	queu.enqueue(0);
	queu.enqueue(1);
	queu.enqueue(2);
	queu.enqueue(3);
	queu.enqueue(4);
	queu.enqueue(5);

	printf("dequeue: %d\n", queu.dequeue());
	printf("dequeue: %d\n", queu.dequeue());
	printf("dequeue: %d\n", queu.dequeue());
	printf("isEmpty: %s\n", queu.empty() ? "true" : "false");
	printf("dequeue: %d\n", queu.dequeue());
	printf("dequeue: %d\n", queu.dequeue());
	printf("dequeue: %d\n", queu.dequeue());
	printf("isEmpty: %s\n", queu.empty() ? "true" : "false");*/

	//ͨ���Լ���Ƶġ������������ԣ�����ͨ�����Passed������ʧ�����Failure
////#define NREDI
//#ifndef NREDI
//	freopen("test_cases.txt", "r", stdin);
//#endif
//	int num_test;
//	scanf("%d", &num_test);
//
//	for (int i = 0; i < num_test; i++)
//	{
//		int n, m; //��������n��m���ֱ�ΪС��ĸ����Լ�������С��20km��С��Ե���Ŀ
//		scanf("%d %d", &n, &m);
//		GraphList graph(n, m);
//		int v1, v2; //��ʾ��v1��v2������С��ľ���С��20km����Ŵ�1��ʼ��
//		for (int i = 0; i < m; i++)
//		{
//			scanf("%d %d", &v1, &v2);
//			graph.insert(v1 - 1, v2 - 1);
//			graph.insert(v2 - 1, v1 - 1);
//		}
//
//		int answer;
//		scanf("%d", &answer);
//
//		//printf("%d\n", graph.bfs());
//		printf("%s\n", (graph.bfs() == answer) ? "Passed" : "Failure");
//	}

	int n, m; //��������n��m���ֱ�ΪС��ĸ����Լ�������С��20km��С��Ե���Ŀ
	scanf("%d %d", &n, &m);
	GraphList graph(n, m);
	int v1, v2; //��ʾ��v1��v2������С��ľ���С��20km����Ŵ�1��ʼ��
	for (int i = 0; i < m; i++)
	{
		scanf("%d %d", &v1, &v2);
		graph.insert(v1 - 1, v2 - 1);
		graph.insert(v2 - 1, v1 - 1);
	}

	printf("%d\n", graph.bfs());

	return 0;
}

