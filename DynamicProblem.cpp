#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

#define INF 10e7

ifstream fin("C:/Users/24664/Documents/GitHub/TSP_Problem/TSP_Problem/Connected_Graph.txt");
const unsigned long long N = 10, M = 1 << (N - 1);
int dis[N][N];//存储城市之间的距离
auto dp = new unsigned int[N][M];

//输入函数
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

//核心函数，求出动态规划dp数组
void dynamic_program() {
	//初始化dp[i][0]
	clock_t start_time = clock();
	for (int i = 0; i < N; i++) {
		dp[i][0] = dis[i][0];
	}
	//求解dp[i][j],先更新列再更新行
	for (int j = 1; j < M; j++) {
		for (int i = 0; i < N; i++) {
			dp[i][j] = INF;
			if (((j >> (i - 1)) & 1) != 1) {
				//集合j中不包含结点i
				for (int k = 1; k < N; k++) {
					if (((j >> (k - 1)) & 1) != 0) {
						//集合j中包含结点k，则尝试用k更新最短路径
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