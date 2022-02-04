#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

using namespace std;

ofstream fout("out.txt");
ifstream fin("C:/Users/24664/Documents/GitHub/TSP_Problem/TSP_Problem/Connected_Graph.txt");

#define INF 10e7
#define RAND_VAR 1
#define RAND_CROSS 1
#define VARIA_TIME 10
#define INIT_VAR_TIME 100
#define BREED_TIME 1000

const int numCity = 24, maxNumRoute = 500;//numCity��ʾ����������maxNumRoute��ʾ��Ⱥ����
int dis[numCity][numCity];//�洢����֮��ľ���
int startNode = 0, route_assemble[maxNumRoute][numCity + 1];//�洢����·����ɵ���Ⱥ���ٶ���Ⱥ��ģΪmaxNumRoute
int shortest_route[numCity + 1];//�洢���ŷ���
int shortest_length = INF/*, stop_count = 0*/;

int getRouteLength(int* route) {
	int sum = 0;
	for (int i = 1; i <= numCity; i++) {
		//ʹ��·�����ȵĵ�������Ϊ���۲���
		sum += dis[route[i - 1]][route[i]];
	}
	return sum;
}

void getShortestRoute() {
	for (int i = 0; i < maxNumRoute; i++) {
		int length = getRouteLength(route_assemble[i]);
		if (length < shortest_length) {
			//stop_count = 0;
			//���µ�ǰ�����ŷ���
			shortest_length = length;
			for (int j = 0; j <= numCity; j++) {
				shortest_route[j] = route_assemble[i][j];
			}
		}
	}
}

void getNewRouteByVariation(int* route) {
	int i = rand() % numCity, j = rand() % numCity, k = rand() % numCity;
	if (route[i] == startNode || route[j] == startNode || route[k] == startNode || i == j || i == k || j == k) {
		//���������ĵ�����������������ͬ����ִ�б���
		return;
	}
	int operation = rand() % 3;//���ѡ��һ�ֱ������
	if (operation == 0) {
		//������������i��j
		int temp = route[i];
		route[i] = route[j];
		route[j] = temp;
	}
	else if (operation == 1) {
		//��λ��,��i��j֮���ƶ���k��
		//�ȱ�֤i < j < k
		if (i > j) { int t = i; i = j; j = k; }
		if (i > k) { int t = i; i = k; k = t; }
		if (j > k) { int t = j; j = k; k = t; }
		int* temp = new int[j - i + 1];
		for (int t = i; t <= j; t++) {
			temp[t - i] = route[t];
		}
		for (int t = j + 1; t <= k; t++) {
			route[t - j + i - 1] = route[t];
		}
		for (int t = 0; t <= j - i; t++) {
			route[k - j + i + t] = temp[t];
		}
		delete[] temp;
	}
	else if (operation == 2) {
		//���÷�����i��j֮���ֵ����
		//�ȱ�֤i < j
		if (i > j) { int t = i; i = j; j = t; }
		int* temp = new int[j - i + 1];
		for (int t = i; t <= j; t++) {
			temp[t - i] = route[t];
		}
		for (int t = j; t >= i; t--) {
			route[t] = temp[j - t];
		}
		delete[] temp;
	}
}

void getNewRouteByCross(int* r1, int* r2) {
	//���������������r1��r2��i ~ j���֣�i��j��1 ~ numCity-1�����֣�0��numCity��ʾstartNode
	int i = rand() % (numCity - 1) + 1, j = rand() % (numCity - 1) + 1;
	if (i > j) { int t = i; i = j; j = t; }
	for (int t = i; t <= j; t++) { int temp = r1[t]; r1[t] = r2[t]; r2[t] = temp; }
	//�жϽ����ĸ���ĺϷ��ԣ����Ϸ������
	bool ifArrivedByR1[numCity + 1], ifArrivedByR2[numCity + 1];
	//�����ó���ʼ�������г��ж�δ������
	memset(ifArrivedByR1, false, numCity + 1);
	memset(ifArrivedByR2, false, numCity + 1);
	ifArrivedByR1[startNode] = ifArrivedByR2[startNode] = true;
	for (int n = 1, m = 1; n < numCity && m < numCity;) {
		//��һ�ξ���ĳ������ʱ��bool������false
		if (!ifArrivedByR1[r1[n]]) { ifArrivedByR1[r1[n++]] = true; }
		if (!ifArrivedByR2[r2[m]]) { ifArrivedByR2[r2[m++]] = true; }
		if (n >= numCity || m >= numCity) { break; }//��ֹ������n��mԽ��
		//���ֻ��һ����true����·����ʱ�����������߶�Ϊtrue����������
		if (ifArrivedByR1[r1[n]] && ifArrivedByR2[r2[m]]) {
			int temp = r1[n];
			r1[n] = r2[m];
			r2[m] = temp;
			n++; m++;
		}
	}
}

void variation() {
	int new_route_assemble[maxNumRoute][numCity + 1];
	for (int i = 0; i < maxNumRoute; i++) {
		for (int j = 0; j <= numCity; j++) {
			new_route_assemble[i][j] = route_assemble[i][j];
		}
	}
	for (int i = 0; i < maxNumRoute * RAND_VAR; i++) {
		//���죬����������ΪRAND_VAR
		int j = rand() % maxNumRoute;
		getNewRouteByVariation(new_route_assemble[j]);
	}
	//��ʤ��̭
	for (int i = 0; i < maxNumRoute; i++) {
		if (getRouteLength(route_assemble[i]) > getRouteLength(new_route_assemble[i])) {
			for (int j = 0; j <= numCity; j++) {
				route_assemble[i][j] = new_route_assemble[i][j];
			}
		}
	}
}

void cross() {
	//��������
	for (int i = 0; i < maxNumRoute * RAND_CROSS; i++) {
		int r1 = rand() % maxNumRoute, r2 = rand() % maxNumRoute;
		if (r1 != r2) {
			//���������·�����н���
			getNewRouteByCross(route_assemble[r1], route_assemble[r2]);
		}
	}
}

void choose() {
	double sum = 0.0, r;
	double route_mark[maxNumRoute];//ÿ��·�������֣�����Ӧ�ȣ�
	int new_route_assemble[maxNumRoute][numCity + 1], count = 0;//�洢����ѡ��֮��ĸ�����Ⱥ
	for (int i = 0; i < maxNumRoute; i++) {
		route_mark[i] = 1 / getRouteLength(route_assemble[i]);
		sum += route_mark[i];
	}
	//����ÿ��·������ÿ�����壩�ķ���
	for (int i = 0; i < maxNumRoute; i++) {
		route_mark[i] /= sum;
	}
	for (int i = 0; i < maxNumRoute; i++) {
		r = ((double)rand()) / RAND_MAX;
		for (int j = 0; j < maxNumRoute; j++) {
			r -= route_mark[j];
			if (r < 0 || j == maxNumRoute - 1) {
				//ѡ�е�j�����뵽����Ⱥ��
				//���ѭ�������һ����δ��ѡ��,Ϊ�˱�����Ⱥ������Ϊ500��ǿ��ѡ�����һ��
				for (int k = 0; k <= numCity; k++) {
					new_route_assemble[count][k] = route_assemble[j][k];
				}
				count++;
				break;
			}
		}
	}
	//���²�������Ⱥ��ֵ
	for (int i = 0; i < maxNumRoute; i++) {
		for (int j = 0; j <= numCity; j++) {
			route_assemble[i][j] = new_route_assemble[i][j];
		}
	}
}

void init_assemble() {
	//��ÿ����ʼ�������50�Σ�ѡȡ���õĸ���
	//����ʹ����Ⱥ�еĳ�ʼ������ã��Ӷ������ҵ����Ž�
	int newRoute[numCity + 1], nextRoute[numCity + 1];
	for (int count = 0; count < maxNumRoute; count++) {
		//������ͨ�����е㣬���ص���ʼ��
		for (int i = 0; i < numCity; i++) {
			newRoute[i] = i;
			nextRoute[i] = i;
		}
		newRoute[numCity] = nextRoute[numCity] = startNode;
		//���ô���ʼ�����
		newRoute[0] = startNode; newRoute[startNode] = 0;
		nextRoute[0] = startNode; nextRoute[startNode] = 0;
		for (int i = 0; i < INIT_VAR_TIME; i++) {
			//��nextNode·�����б���
			getNewRouteByVariation(nextRoute);
			if (getRouteLength(newRoute) >= getRouteLength(nextRoute)) {
				//��������nextRoute��·���̣����丳ֵ��newRoute֮��
				for (int j = 0; j <= numCity; j++) { newRoute[j] = nextRoute[j]; }
			}
			//��˽���100�Σ��������ŵĳ�ʼ��Ⱥ
		}
		for (int i = 0; i <= numCity; i++) {
			route_assemble[count][i] = newRoute[i];
		}
	}
}

void input() {
	//fin >> startNode; //��ʼ��Ĭ����Ϊ0
	//��ʼ������֮�����ΪINF����
	//���б��Ϊ0 ~ n - 1
	for (int i = 0; i < numCity; i++) {
		for (int j = 0; j < numCity; j++) {
			dis[i][j] = INF;
		}
	}
	//��ʼ�����·����ȫΪ-1
	//��ʼ�������Լ����Լ��ľ���Ϊ0
	for (int i = 0; i < numCity; i++) {
		shortest_route[i] = -1;
		dis[i][i] = 0;
	}
	shortest_route[numCity] = -1;
	//��txt�ļ��ж�ȡ����֮��ľ���
	while (!fin.eof()) {
		int n1, n2, d;
		fin >> n1 >> n2 >> d;
		//���б��Ϊ0 ~ n-1
		dis[n1][n2] = d;
		dis[n2][n1] = d;
	}
}

void output() {
	cout << "\n" << shortest_length << "\n";
	//for (int i = 0; i < numCity; i++) {
	//	cout << shortest_route[i] << "-->";
	//}
	//cout << shortest_route[numCity] << "\n";
}

void genetic_algorithm() {
	clock_t start_time = clock();
	init_assemble();//���ɳ�ʼ��Ⱥ
	/*׼��������һ���ķ�ֳ*/
	for (int i = 0; i < BREED_TIME; i++) {
		cross();//��������
		for (int j = 0; j < VARIA_TIME; j++) {
			variation();//��������
		}
		choose();//ѡ������

		//����������50�ζ����������Ž⣬��ֹͣ����
		getShortestRoute();//��ȡ��ǰ��Ⱥ�е����Ÿ���
		fout << shortest_length << "\n";
	}
	clock_t end_time = clock();
	cout << (double(end_time) - double(start_time)) / 1000.0;
}

int main() {
	srand((int)time(NULL));
	input();
	genetic_algorithm();
	output();
	return 0;
}