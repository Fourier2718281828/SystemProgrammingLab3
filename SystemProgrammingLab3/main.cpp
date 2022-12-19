#include <iostream>
#include "Matrix.h"
#include "ParallelMultiplication.h"
using std::cout;
using std::endl;

#include <vector>
#include <algorithm>
#include <ctime>

#define M 400
#define N 200
#define K 300

void fillWithRandoms(std::vector<int>& v)
{
	std::srand(unsigned(std::time(nullptr)));
	//std::vector<int> v(size);
	std::generate(v.begin(), v.end(), std::rand);
}

int main(void)
{
	std::vector<int> a(M * N);
	std::vector<int> b(N * K);

	fillWithRandoms(a);
	fillWithRandoms(b);

	Matrix<int, M, N>* m1 = new Matrix<int, M, N>(a);
	Matrix<int, N, K>* m2 = new Matrix<int, N, K>(b);

	auto res1 = parallelMultiply(*m1, *m2);
	auto res2 = new Matrix<int, M, K>(*m1 * *m2);

	cout << (*res1 == *res2) << endl;
	return 0;
}