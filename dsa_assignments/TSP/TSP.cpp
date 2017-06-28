// TSP.cpp : �������̨Ӧ�ó������ڵ㡣
//����������

#include <cstdio>
#include <cstdlib>

enum VStatus { UNDISCOVERED, DISCOVERED };
const int MAX = 1000000;

//������
struct Vertex {
	VStatus status; //�ڵ�״̬
	int longestPath; //�Ե�ǰ�ڵ�Ϊstarting point���·���ϵĽڵ���

	Vertex(VStatus status = UNDISCOVERED, int longestPath = 0):status(status), longestPath(longestPath){}
};

//����
struct Edge {
	int data;
	int* next;
};

//ʹ��adjacency matrixʵ��graph
struct GraphMatrix {
	int n;// �������Ŀ
	int e;//�ߵ���Ŀ
	Vertex* vertexes;
	int** edges;

	//n�Ƕ�����Ŀ��m�Ǳߵ���Ŀ
	GraphMatrix(int n, int m) :n(n), e(m) {
		vertexes = new Vertex[n];
		edges = new int*[n];
		//�Ѿ����������Ԫ�ض���ʼ��Ϊ0
		for (int i = 0; i < n; i++)
		{
			edges[i] = new int[n];
			for (int j = 0; j < n; j++)
			{
				edges[i][j] = 0;
			}
		}
	}
	~GraphMatrix() {
		delete vertexes;
		for (int i = 0; i < n; i++)
		{
			delete[] edges[i];
		}
	}

	//�����i��j�ı�
	void insert(int i, int j) {
		edges[i][j] = 1;
	}

	//���Ҷ���i������ڶ���j����һ���ھӣ�������ң�
	int nextNbr(int i, int j) {
		while ((-1 < j) && (1 != edges[i][--j]));
		return j;
	}
	int firstNbr(int i) {
		return nextNbr(i, n);
	}

	//debug
	void displayEdges() {
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", edges[i][j]);
			}
			printf("\n");
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
	//����
	int DFS(int v) {
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
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
		GraphMatrix graph(4, 3);
		graph.insert(1 - 1, 4 - 1);
		graph.insert(2 - 1, 4 - 1);
		graph.insert(4 - 1, 3 - 1);
		graph.displayEdges();

	/*	GraphMatrix graph(12, 10);
		graph.insert(2 - 1, 1 - 1);
		graph.insert(3 - 1, 4 - 1);
		graph.insert(5 - 1, 4 - 1);
		graph.insert(6 - 1, 8 - 1);
		graph.insert(7 - 1, 8 - 1);
		graph.insert(8 - 1, 9 - 1);
		graph.insert(8 - 1, 10 - 1);
		graph.insert(5 - 1, 11 - 1);
		graph.insert(5 - 1, 8 - 1);
		graph.insert(9 - 1, 12 - 1);
		graph.displayEdges();*/

		int temp;
		printf("first: %d\n", temp = graph.firstNbr(1 - 1));
		printf("%d\n", temp = graph.nextNbr(1 - 1, temp));
		printf("%d\n", temp = graph.nextNbr(1 - 1, temp));
		printf("%d\n", temp = graph.nextNbr(1 - 1, temp));
		printf("%d\n", temp = graph.nextNbr(1, temp));

		printf("first: %d\n", temp = graph.firstNbr(2 - 1));

		printf("output: %d\n", graph.tSort());

		//int n, m; //nΪ��ׯ��Ŀ��mΪ��·����Ŀ
		//scanf("%d %d", &n, &m);
		//GraphMatrix graph(n, m);
		//int v1, v2; //��ʾ��v1��v2��·
		//for (int i = 0; i < m; i++)
		//{
		//	scanf("%d %d", &v1, &v2);
		//	graph.insert(v1 - 1, v2 - 1);
		//}
		//printf("%d\n", graph.tSort());


		return 0;
	}

