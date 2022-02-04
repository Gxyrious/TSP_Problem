#include <iostream>
#include <fstream>
#include <stdlib.h>

//图的稠密因子
#define DENSITY_FACTOR 0.4
#define INF 10e7
using namespace std;

ofstream fout("Connected_Graph.txt");
const int N = 10;
int graph[N][N];

void init_graph() {
	for (int i = 0; i < N; i++) {
		graph[i][i] = 0;
		for (int j = i + 1; j < N; j++) {
			if (double(rand()) / RAND_MAX < DENSITY_FACTOR) {
				//连通
				graph[i][j] = rand();
				graph[j][i] = graph[i][j];
			}
			else {
				//不连通
				graph[i][j] = INF;
				graph[j][i] = INF;
			}
		}
	}
}

void Floyd() {
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (graph[i][k] + graph[k][j] < graph[i][j]) {
					graph[i][j] = graph[i][k] + graph[k][j];
				}
			}
		}
	}
}

bool if_graph_connected() {
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (graph[i][j] == INF) {
				//存在最短路径为无穷的两个结点
				//说明生成的graph不连通
				return false;
			}
		}
	}
	return true;
}

void output() {
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (graph[i][j] != INF) {
				fout << i << " " << j << " "<<graph[i][j] << "\n";
			}
		}
	}
}

int main() {
	srand((unsigned)time(NULL));
	do {
	//初始化图
	init_graph();
	//使用Floyd算法求解所有结点之间的最短路径
	Floyd();
	} while (!if_graph_connected());
	output();
	return 0;
}