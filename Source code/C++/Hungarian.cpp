#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

const double INF = std::numeric_limits<double>::max();

// Функция для решения задачи о назначениях
double hungarianAlgorithm(const std::vector<std::vector<double>>& a) {
	int n = a.size();

	std::vector<double> u (n+1), v (n+1);
	std::vector<int> p (n+1), way (n+1);
	for (int i=1; i<=n; ++i) {
		p[0] = i;
		int j0 = 0;
		std::vector<double> minv (n+1, INF);
		std::vector<char> used (n+1, false);
		do {
			used[j0] = true;
			int i0 = p[j0],  j1;
			double delta = INF;
			for (int j=1; j<=n; ++j)
				if (!used[j]) {
					double cur = a[i0-1][j-1]-u[i0]-v[j];
					if (cur < minv[j])
						minv[j] = cur,  way[j] = j0;
					if (minv[j] < delta)
						delta = minv[j],  j1 = j;
				}
			for (int j=0; j<=n; ++j)
				if (used[j])
					u[p[j]] += delta,  v[j] -= delta;
				else
					minv[j] -= delta;
			j0 = j1;
		} while (p[j0] != 0);
		do {
			int j1 = way[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}
	std::vector<double> ans (n+1);
	for (int j=1; j<=n; ++j)
		ans[p[j]] = j;
	return -v[0];
}

int main() {
	int n;
	std::cout << "Введите размер: ";
	std::cin >> n;

	std::vector<std::vector<double>> cost(n, std::vector<double>(n));
	std::cout << "Введите матрицу стоимостей:\n";
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> cost[i][j];
		}
	}

	int result = hungarianAlgorithm(cost);

	std::cout << "Минимальная стоимость: " << result << std::endl;

	return 0;
}
