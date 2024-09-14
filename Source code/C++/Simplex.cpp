#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

const int INF = std::numeric_limits<int>::max();
int N; // Глобальная переменная N для размерности задачи

// Функция для вывода текущей симплекс-таблицы
void printTable(const std::vector<std::vector<double>>& table, const std::vector<int>& basis) {
	std::cout << "Симплекс-таблица:" << std::endl;
	std::cout << "\t";
	for (int i = 0; i < N * N; ++i) {
		std::cout << 'x' << i + 1 << "\t";
	}
	std::cout << std::endl << 'z' << "\t";
	int p = 0;
	for (const auto& row : table) {
		for (double value : row) {
			std::cout << value << "\t";
		}
		std::cout << std::endl;
		if (basis[p] != -1) {
			std::cout << 'x' << basis[p] + 1 << "\t";
		} else {
			std::cout << "\t";
		}
		p++;
	}
	std::cout << "Базисные переменные: ";
	for (int i = 0; i < basis.size(); ++i) {
		if (basis[i] != -1) {
			std::cout << "x" << basis[i] + 1 << " ";
		}
	}
	std::cout << std::endl << std::endl;
}

// Функция для нахождения разрешающего столбца
int findPivotColumn(const std::vector<std::vector<double>>& table, const std::vector<int>& basis) {
	int pivotCol = -1;
	double maxdelta = 0;
	for (int p = 0; p < N * N; p++) {
		double delta = -table[0][p];
		for (int i = 1; i <= basis.size(); i++) {
			delta += table[0][basis[i - 1]] * table[i][p];
		}
		if (delta > maxdelta) {
			maxdelta = delta;
			pivotCol = p;
		}
	}
	return pivotCol;
}

// Функция для нахождения разрешающей строки
int findPivotRow(const std::vector<std::vector<double>>& table, int pivotCol) {
	int pivotRow = -1;
	double minRatio = INF;
	for (int i = 1; i <= 2 * N; ++i) {
		if (table[i][pivotCol] > 0) {
			double ratio = table[i].back() / table[i][pivotCol];
			if (ratio < minRatio) {
				minRatio = ratio;
				pivotRow = i;
			}
		}
	}
	return pivotRow;
}

// Функция для выполнения симплекс-поворота
void pivot(std::vector<std::vector<double>>& table, std::vector<int>& basis, int pivotRow, int pivotCol) {
	double pivotValue = table[pivotRow][pivotCol];

	// Делим строку разрешающего элемента на сам элемент
	for (double& value : table[pivotRow]) {
		value /= pivotValue;
	}

	// Обновляем остальные строки
	for (int i = 1; i <= 2 * N; ++i) {
		if (i != pivotRow) {
			double factor = table[i][pivotCol];
			for (int j = 0; j < table[0].size(); ++j) {
				table[i][j] -= factor * table[pivotRow][j];
			}
		}
	}

	// Обновляем базисные переменные
	basis[pivotRow - 1] = pivotCol;
}

// Основная функция для решения задачи о назначениях с использованием симплекс-метода
void solveAssignmentProblem(std::vector<std::vector<double>>& cost) {
	// Инициализация симплекс-таблицы и базисных переменных
	std::vector<std::vector<double>> table(2 * N + 1, std::vector<double>(N * N + 1, 0));
	std::vector<int> basis(2 * N, -1);

	// Заполняем строку целевой функции
	for (int j = 0; j < N * N; ++j) {
		table[0][j] = cost[j / N][j % N];
	}

	// Заполняем таблицу ограничений
	for (int i = 1; i <= N; ++i) {
		for (int j = 0; j < N; ++j) {
			table[i][(i - 1) * N + j] = 1;
		}
		table[i][N * N] = 1;
	}
	for (int i = N + 1; i <= 2 * N; ++i) {
		for (int j = 0; j < N * N; ++j) {
			table[i][j] = ((j + 1) % N == (i - N) % N) ? 1 : 0;
		}
		table[i][N * N] = 1;
	}
	printTable(table, basis);
	// Заполнение массива basis
	for (int i = 0; i < N; ++i) {
		basis[i] = i * N;
	}
	int currentIndex = N;
	for (int i = 1; i <= N * N && currentIndex < 2 * N; ++i) {
		if (std::find(basis.begin(), basis.begin() + N, i) == basis.begin() + N) {
			basis[currentIndex++] = i;
		}
	}

	std::cout << "Начальный базис построен" << std::endl;
	printTable(table, basis);

	// Симплекс-метод
	int it=1;
	while (true) {


		int pivotCol = findPivotColumn(table, basis);
		if (pivotCol < 0) {
			break;
		}
		int pivotRow = findPivotRow(table, pivotCol);
		if (pivotRow == -1) {
			std::cout << "Решение не существует (неограниченная задача)." << std::endl;
			return;
		}

		pivot(table, basis, pivotRow, pivotCol);
		std::cout << "Итерация " << it << std::endl;
		it++;
		printTable(table, basis);
	}

	// Вывод оптимального решения
	double totalSum = 0;
	std::cout << "Оптимальное решение найдено:" << std::endl;
	for (int i = 0; i < basis.size(); ++i) {
		if (table[i + 1][N * N] != 0) {
			int col = basis[i] % N;
			int row = basis[i] / N;
			totalSum += cost[row][col];  // Добавляем соответствующее значение в сумму
			std::cout << "Работник " << row + 1 << " назначен на задачу " << col + 1 << std::endl;
		}
	}
	std::cout << "Сумма значений, соответствующих номерам в базисе: " << totalSum << std::endl;
}

int main() {
	// Ввод размерности
	std::cout << "Введите размер задачи N: ";
	std::cin >> N;

	// Инициализация таблицы стоимости
	std::vector<std::vector<double>> cost(N, std::vector<double>(N));

	std::cout << "Введите элементы таблицы стоимости:" << std::endl;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			std::cin >> cost[i][j];
		}
	}

	solveAssignmentProblem(cost);

	return 0;
}
