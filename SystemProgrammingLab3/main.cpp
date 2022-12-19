#include <iostream>
#include <chrono>
#include "Matrix.h"
#include "ParallelMultiplication.h"
using std::cout;
using std::endl;

#include <vector>
#include <algorithm>
#include <ctime>

#define M 1000
#define N 1000
#define K 1000

void fillWithRandoms(std::vector<int>& v)
{
	std::srand(unsigned(std::time(nullptr)));
	std::generate(v.begin(), v.end(), std::rand);
}

int main(void)
{
	/*Matrix<int, 4, 2> aa(std::vector<int> { 1, 2, 3, 4, 5, 6, 7, 8 });
	Matrix<int, 2, 4> bb = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8 };
	cout << aa << endl;
	cout << bb << endl;
	cout << aa * bb << endl;*/

	std::vector<int> a(M * N);
	std::vector<int> b(N * K);

	fillWithRandoms(a);
	fillWithRandoms(b);

	Matrix<int, M, N>* m1 = new Matrix<int, M, N>(a);
	Matrix<int, N, K>* m2 = new Matrix<int, N, K>(b);

	auto start = std::chrono::steady_clock::now();

	auto res1 = parallelMultiply(*m1, *m2);
	auto end = std::chrono::steady_clock::now();
	auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	cout << "Parallel: " << time1 << " ms" << endl;

	auto start2 = std::chrono::steady_clock::now();
	auto res2 = heapMultiply(*m1, *m2);
	auto end2 = std::chrono::steady_clock::now();
	auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();

	cout << "Single-threaded: " << time2 << " ms" << endl;
	cout << "Total improvement : " << static_cast<double>(time2) / time1 << " times" << endl;

	cout << std::boolalpha << "Result correct : " <<  (*res1 == *res2) << endl;
	return 0;
}