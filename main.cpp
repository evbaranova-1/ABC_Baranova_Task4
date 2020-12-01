// Вариант 2, Баранова Екатерина

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

vector<vector<int> > matrix; // Матрица
vector<int> permutation; // Перестановка
int n, threads_count; // Размер матрицы и количество потоков
long long determinant; // Определитель матрицы - результат

// Метод для подсчета количества инверсий в перестановке
int inversion_rate(const int* p) {
	int ans = 0;
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
			if (p[i] > p[j])
				++ans;
	return ans;
}

// Метод для потока
void thread_task(const int* p) {
	// Считаем очередное слагаемое в определителе
	int inversions = inversion_rate(p);
	int ans = 1;
	for (int i = 0; i < n; ++i)
		ans *= matrix[i][p[i]];
	determinant += inversions % 2 ? -ans : ans;
}

int factorial(int i) {
	int ans = 1;
	for (int j = 2; j <= i; ++j)
		ans *= j;
	return ans;
}

int main() {
	setlocale(0, "ru_RU");
	// Считывем все необходимое
	while (n <= 0 || n > 10) {
		cout << "Введите размер матрицы (от 1 до 10): ";
		cin >> n;
	}
	while (threads_count <= 0 || threads_count > 100) {
		cout << "Введите количество потоков (от 1 до 100): ";
		cin >> threads_count;
	}
	cout << "Значения элементов матрицы должны лежать в пределе от -9 до 9!" << endl;
	matrix.resize(n);
	for (int i = 0; i < n; ++i) {
		matrix[i].resize(n);
		for (int j = 0; j < n; ++j) {
			do {
				cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
				cin >> matrix[i][j];
			} while (matrix[i][j] < -9 || matrix[i][j] > 9);
		}
	}

	// Выводим матрицу
	cout << "Ваша матрица А:" << endl << endl;
	for (int i = 0; i < n; ++i) {
		cout << "( ";
		for (int j = 0; j < n; ++j)
			cout << (matrix[i][j] < 0 ? "" : " ") << matrix[i][j] << " ";
		cout << ")" << endl;
	}

	// Инициализируем глобальную перестановку для потоков
	permutation.resize(n);
	for (int i = 0; i < n; ++i)
		permutation[i] = i;

	int i, x = factorial(n);
	int* nums = new int[n];

#pragma omp parallel for private(i, nums) shared(permutation) schedule(dynamic) num_threads(threads_count)
	for (i = 0; i < x; ++i) {
#pragma omp critical
		{
			nums = new int[n];
			copy(permutation.begin(), permutation.end(), nums);
			next_permutation(permutation.begin(), permutation.end());
		}
		thread_task(nums);
	}
	delete[] nums;

	// Выводим определитель
	cout << endl << "Опредеделитель матрицы |A| = " <<
		determinant;

	return 0;
}
