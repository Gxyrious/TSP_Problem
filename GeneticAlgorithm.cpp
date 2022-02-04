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

const int numCity = 24, maxNumRoute = 500;//numCity表示城市数量，maxNumRoute表示种群数量
int dis[numCity][numCity];//存储城市之间的距离
int startNode = 0, route_assemble[maxNumRoute][numCity + 1];//存储所有路径组成的种群，假定种群规模为maxNumRoute
int shortest_route[numCity + 1];//存储最优方案
int shortest_length = INF/*, stop_count = 0*/;

int getRouteLength(int* route) {
	int sum = 0;
	for (int i = 1; i <= numCity; i++) {
		//使用路径长度的倒数和作为评价参数
		sum += dis[route[i - 1]][route[i]];
	}
	return sum;
}

void getShortestRoute() {
	for (int i = 0; i < maxNumRoute; i++) {
		int length = getRouteLength(route_assemble[i]);
		if (length < shortest_length) {
			//stop_count = 0;
			//更新当前的最优方案
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

void getNewRouteByCross(int* r1, int* r2) {
	//随机交换两个个体r1和r2的i ~ j部分，i、j是1 ~ numCity-1的数字，0和numCity表示startNode
	int i = rand() % (numCity - 1) + 1, j = rand() % (numCity - 1) + 1;
	if (i > j) { int t = i; i = j; j = t; }
	for (int t = i; t <= j; t++) { int temp = r1[t]; r1[t] = r2[t]; r2[t] = temp; }
	//判断交叉后的个体的合法性，不合法则调整
	bool ifArrivedByR1[numCity + 1], ifArrivedByR2[numCity + 1];
	//先设置除起始点外所有城市都未被访问
	memset(ifArrivedByR1, false, numCity + 1);
	memset(ifArrivedByR2, false, numCity + 1);
	ifArrivedByR1[startNode] = ifArrivedByR2[startNode] = true;
	for (int n = 1, m = 1; n < numCity && m < numCity;) {
		//第一次经过某个城市时，bool数组是false
		if (!ifArrivedByR1[r1[n]]) { ifArrivedByR1[r1[n++]] = true; }
		if (!ifArrivedByR2[r2[m]]) { ifArrivedByR2[r2[m++]] = true; }
		if (n >= numCity || m >= numCity) { break; }//防止交换后n、m越界
		//如果只有一个是true，该路径暂时阻塞；当两者都为true，交换两者
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
		//变异，假设变异概率为RAND_VAR
		int j = rand() % maxNumRoute;
		getNewRouteByVariation(new_route_assemble[j]);
	}
	//优胜劣汰
	for (int i = 0; i < maxNumRoute; i++) {
		if (getRouteLength(route_assemble[i]) > getRouteLength(new_route_assemble[i])) {
			for (int j = 0; j <= numCity; j++) {
				route_assemble[i][j] = new_route_assemble[i][j];
			}
		}
	}
}

void cross() {
	//交叉运算
	for (int i = 0; i < maxNumRoute * RAND_CROSS; i++) {
		int r1 = rand() % maxNumRoute, r2 = rand() % maxNumRoute;
		if (r1 != r2) {
			//随机将两条路径进行交叉
			getNewRouteByCross(route_assemble[r1], route_assemble[r2]);
		}
	}
}

void choose() {
	double sum = 0.0, r;
	double route_mark[maxNumRoute];//每条路径的评分（即适应度）
	int new_route_assemble[maxNumRoute][numCity + 1], count = 0;//存储经过选择之后的更优种群
	for (int i = 0; i < maxNumRoute; i++) {
		route_mark[i] = 1 / getRouteLength(route_assemble[i]);
		sum += route_mark[i];
	}
	//计算每条路径（即每个个体）的分数
	for (int i = 0; i < maxNumRoute; i++) {
		route_mark[i] /= sum;
	}
	for (int i = 0; i < maxNumRoute; i++) {
		r = ((double)rand()) / RAND_MAX;
		for (int j = 0; j < maxNumRoute; j++) {
			r -= route_mark[j];
			if (r < 0 || j == maxNumRoute - 1) {
				//选中第j个加入到新种群中
				//如果循环到最后一个仍未被选中,为了保持种群个数仍为500，强制选中最后一个
				for (int k = 0; k <= numCity; k++) {
					new_route_assemble[count][k] = route_assemble[j][k];
				}
				count++;
				break;
			}
		}
	}
	//将新产生的种群赋值
	for (int i = 0; i < maxNumRoute; i++) {
		for (int j = 0; j <= numCity; j++) {
			route_assemble[i][j] = new_route_assemble[i][j];
		}
	}
}

void init_assemble() {
	//对每个初始个体变异50次，选取更好的个体
	//这样使得种群中的初始基因更好，从而容易找到更优解
	int newRoute[numCity + 1], nextRoute[numCity + 1];
	for (int count = 0; count < maxNumRoute; count++) {
		//先依次通过所有点，最后回到起始点
		for (int i = 0; i < numCity; i++) {
			newRoute[i] = i;
			nextRoute[i] = i;
		}
		newRoute[numCity] = nextRoute[numCity] = startNode;
		//设置从起始点出发
		newRoute[0] = startNode; newRoute[startNode] = 0;
		nextRoute[0] = startNode; nextRoute[startNode] = 0;
		for (int i = 0; i < INIT_VAR_TIME; i++) {
			//将nextNode路径进行变异
			getNewRouteByVariation(nextRoute);
			if (getRouteLength(newRoute) >= getRouteLength(nextRoute)) {
				//如果变异后nextRoute的路径短，则将其赋值给newRoute之中
				for (int j = 0; j <= numCity; j++) { newRoute[j] = nextRoute[j]; }
			}
			//如此进行100次，产生较优的初始种群
		}
		for (int i = 0; i <= numCity; i++) {
			route_assemble[count][i] = newRoute[i];
		}
	}
}

void input() {
	//fin >> startNode; //起始点默认置为0
	//初始化城市之间距离为INF无穷
	//城市编号为0 ~ n - 1
	for (int i = 0; i < numCity; i++) {
		for (int j = 0; j < numCity; j++) {
			dis[i][j] = INF;
		}
	}
	//初始化最短路径，全为-1
	//初始化城市自己到自己的距离为0
	for (int i = 0; i < numCity; i++) {
		shortest_route[i] = -1;
		dis[i][i] = 0;
	}
	shortest_route[numCity] = -1;
	//从txt文件中读取城市之间的距离
	while (!fin.eof()) {
		int n1, n2, d;
		fin >> n1 >> n2 >> d;
		//城市编号为0 ~ n-1
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
	init_assemble();//生成初始种群
	/*准备进行下一代的繁殖*/
	for (int i = 0; i < BREED_TIME; i++) {
		cross();//交叉算子
		for (int j = 0; j < VARIA_TIME; j++) {
			variation();//变异算子
		}
		choose();//选择算子

		//若连续迭代50次都不产生更优解，则停止迭代
		getShortestRoute();//获取当前种群中的最优个体
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