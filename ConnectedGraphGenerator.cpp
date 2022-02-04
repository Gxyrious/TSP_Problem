#include <iostream>
#include <fstream>
#include <stdlib.h>

//ͼ�ĳ�������
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
				//��ͨ
				graph[i][j] = rand();
				graph[j][i] = graph[i][j];
			}
			else {
				//����ͨ
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
				//�������·��Ϊ������������
				//˵�����ɵ�graph����ͨ
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
	//��ʼ��ͼ
	init_graph();
	//ʹ��Floyd�㷨������н��֮������·��
	Floyd();
	} while (!if_graph_connected());
	output();
	return 0;
}