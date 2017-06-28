// TSP.cpp : �������̨Ӧ�ó������ڵ㡣
//����������

#include <cstdio>
#include <cstdlib>

enum VStatus { UNDISCOVERED, DISCOVERED };

//������
struct Vertex {
	VStatus status; //�ڵ�״̬
	int longestPath; //�Ե�ǰ�ڵ�Ϊstarting point���·���ϵĽڵ���

	Vertex(VStatus status = UNDISCOVERED, int longestPath = 0) :status(status), longestPath(longestPath) {}
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

	//ͨ����������õ�����������Ľ⣨TSP��
	int tSort() {
		int startPnt = rand() % n;
		int v = startPnt; //v�൱���α������
		int max = 0; //���ڼ�¼TSP�Ľ�
		do {
			if (UNDISCOVERED == vertexes[v].status) {
				int temp = DFS(v);
				max = (temp > max) ? temp : max;
			}
		} while (startPnt != (v = (++v % n)));
		return max;
	}
	//����(������ͨ��)
	int DFS(int v) {
		for (int u = nextNbr(v); -1 < u; u = nextNbr(v)) {
			switch (vertexes[u].status)
			{
			case UNDISCOVERED:
				vertexes[v].longestPath = ((DFS(u) + 1) > vertexes[v].longestPath) ? \
					(DFS(u) + 1) : vertexes[v].longestPath;
				break;
			case DISCOVERED:
				return (vertexes[u].longestPath + 1) > vertexes[v].longestPath ? \
					(vertexes[v].longestPath = vertexes[u].longestPath + 1) : vertexes[v].longestPath;
				break;
			}

		}
		return (0 == vertexes[v].longestPath) ? (vertexes[v].longestPath = 1) : vertexes[v].longestPath;
	}
};



int main()
{
	int n, m; //nΪ��ׯ��Ŀ��mΪ��·����Ŀ
	scanf("%d %d", &n, &m);
	GraphList graph(n, m);
	int v1, v2; //��ʾ��v1��v2��·
	for (int i = 0; i < m; i++)
	{
		scanf("%d %d", &v1, &v2);
		graph.insert(v1 - 1, v2 - 1);
	}
	printf("%d\n", graph.tSort());


	return 0;
}

