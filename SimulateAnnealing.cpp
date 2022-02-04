#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>

using namespace std;

ofstream fout("out.txt");
ifstream fin("C:/Users/24664/Documents/GitHub/TSP_Problem/TSP_Problem/Connected_Graph.txt");

#define INF 10e7
#define CONST_R 0.99999
//#define CONST_R 0.999

const int numCity = 200;
int dis[numCity][numCity];
int startNode = 0;
int shortest_length = INF;

int route[numCity + 1], newRoute[numCity + 1];

int getRouteLength(int* route) {
	int sum = 0;
	for (int i = 1; i <= numCity; i++) {
		sum += dis[route[i - 1]][route[i]];
	}
	return sum;
}

void getNewRoute(int* route) {
	int i = rand() % numCity, j = rand() % numCity, k = rand() % numCity;
	if (route[i] == startNode || route[j] == startNode || route[k] == startNode || i == j || i == k || j == k) {
		//如果随机到的点是起点或者两个点相同，则不执行变异
		return;
	}
	int operation = rand() % 3;//随机选择一种变异操作
	if (operation == 0) {
		//交换法，交换i和j
		int temp = route[i];
		route[i] = route[j];
		route[j] = temp;
	}
	else if (operation == 1) {
		//移位法,将i和j之间移动至k后
		//先保证i < j < k
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
		//倒置法，将i和j之间的值倒置
		//先保证i < j
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

void getRandomRoute(int* route) {
	for (int i = 1; i < numCity; i++) {
		route[i] = i;
	}
	route[0] = route[numCity] = startNode;
	route[startNode] = 0;
	for (int i = 1; i < numCity - 1; i++) {
		int j = 1 + i + rand() % (numCity - i - 1);
		int temp = route[i];
		route[i] = route[j];
		route[j] = temp;
	}
}

void init_route() {
	//迭代10次，产生较优的初始线路
	getRandomRoute(route);
	for (int i = 0; i < 10; i++) {
		getRandomRoute(newRoute);
		if (getRouteLength(newRoute) < getRouteLength(route)) {
			for (int j = 1; j < numCity; j++) {
				route[j] = newRoute[j];
			}
		}
	}
}

//输入函数
void input() {
	//初始化城市之间距离为INF无穷
	//城市编号为0 ~ n-1
	for (int i = 0; i < numCity; i++) {
		for (int j = 0; j < numCity; j++) {
			dis[i][j] = INF;
		}
	}
	//初始化最短路径，全为-1
	//初始化城市自己到自己的距离为0
	for (int i = 0; i < numCity; i++) {
		dis[i][i] = 0;
	}
	//从txt文件中读取城市之间的距离
	while (!fin.eof()) {
		int n1, n2,d;
		fin >> n1 >> n2 >> d;
		//城市编号为0 ~ n-1
		dis[n1][n2] = d;
		dis[n2][n1] = d;
	}
}

void output() {
	cout << "\n" << getRouteLength(route) << "\n";
	//以下函数是输出最短回路
	//for (int i = 0; i < numCity; i++) {
	//	cout << route[i]<< "-->";
	//}
	//cout << route[numCity] << "\n";
}

void simulated_annealing_algorithm() {
	clock_t start_time = clock();
	init_route();//生成初始种群
	double SA_T = 0 /*= numCity*RAND_MAX*/, SA_END_T = 50, SA_R = CONST_R;

	int oldLength = getRouteLength(route), newLength = 0;
	for (int i = 0; i < 100; i++) {
		getNewRoute(route);
		newLength = getRouteLength(route);
		SA_T += abs(oldLength - newLength);
		oldLength = newLength;
	}
	SA_T /= 100;
	while (SA_T>SA_END_T) {
		for (int i = 1; i < numCity; i++) {
			newRoute[i] = route[i];
		}
		for (int i = 0; i < 1; i++) {
			getNewRoute(newRoute);
		}
		int oldLength = getRouteLength(route), newLength = getRouteLength(newRoute);
		double diff = double(getRouteLength(newRoute)) - double(getRouteLength(route));
		double t = double(rand()) / RAND_MAX;
		if (diff<0 || exp(-diff / SA_T)>t) {
			//更新最优解
			shortest_length = newLength;
			for (int i = 1; i < numCity; i++) {
				route[i] = newRoute[i];
			}
		}
		//fout << shortest_length << "\n";
		SA_T *= SA_R;
	}
	clock_t end_time = clock();
	cout << (double(end_time) - double(start_time)) / 1000.0;
}

int main() {
	srand((int)time(NULL));
	input();
	simulated_annealing_algorithm();
	output();
	return 0;
}