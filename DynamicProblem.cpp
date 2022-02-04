#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

#define INF 10e7

ifstream fin("C:/Users/24664/Documents/GitHub/TSP_Problem/TSP_Problem/Connected_Graph.txt");
const unsigned long long N = 10, M = 1 << (N - 1);
int dis[N][N];//�洢����֮��ľ���
auto dp = new unsigned int[N][M];

//���뺯��
void input() {
	//fin >> startNode;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			dis[i][j] = INF;
		}
	}
	for (int i = 0; i < N; i++) {
		dis[i][i] = 0;
	}
	while (!fin.eof()) {
		int n1, n2, d;
		fin >> n1 >> n2 >> d;
		dis[n1][n2] = d;
		dis[n2][n1] = d;
	}
}

//���ĺ����������̬�滮dp����
void dynamic_program() {
	//��ʼ��dp[i][0]
	clock_t start_time = clock();
	for (int i = 0; i < N; i++) {
		dp[i][0] = dis[i][0];
	}
	//���dp[i][j],�ȸ������ٸ�����
	for (int j = 1; j < M; j++) {
		for (int i = 0; i < N; i++) {
			dp[i][j] = INF;
			if (((j >> (i - 1)) & 1) != 1) {
				//����j�в��������i
				for (int k = 1; k < N; k++) {
					if (((j >> (k - 1)) & 1) != 0) {
						//����j�а������k��������k�������·��
						if (dp[i][j] > dis[i][k] + dp[k][j ^ (1 << (k - 1))]) {
							dp[i][j] = dis[i][k] + dp[k][j ^ (1 << (k - 1))];
						}
					}
				}
			}
		}
	}
	clock_t end_time = clock();
	cout << (double(end_time) - double(start_time)) / 1000.0
		<< "\n" << dp[0][M - 1];
}

int main() {
	input();
	dynamic_program();
	delete[] dp;
	return 0;
}