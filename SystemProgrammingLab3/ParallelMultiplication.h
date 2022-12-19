#pragma once
#include "Matrix.h"
#include <thread>
#include <vector>


#define CORES 4

template<typename _T, IndexType _M, IndexType _N>
std::vector<std::pair<IndexType, IndexType>> divisions(const Matrix<_T, _M, _N>& A)
{
	std::vector<std::pair<IndexType, IndexType>> res;
	res.reserve(CORES);
	IndexType v = _N / CORES;
	IndexType col = 0;

	while (_N - col >= v)
	{
		auto nxt = col + v;
		res.push_back(std::make_pair(col, nxt));
		col = nxt;
	}

	if (col < _N)
	{
		auto [a, b] = res.back();
		res.pop_back();
		res.push_back(std::make_pair(a, _N));
	}

	return res;
}

template<typename _T, IndexType _M, IndexType _N, IndexType _K>
void partialMultiply(Matrix<_T, _M, _K>& res, 
	const Matrix<_T, _M, _N>& A, const Matrix<_T, _N, _K>& B,
	IndexType left, IndexType right)
{
	for (IndexType i = 0; i < _M; ++i)
	{
		for (IndexType j = left; j < right; ++j)
		{
			_T curr_val = 0;
			for (IndexType k = 0; k < _N; ++k)
			{
				curr_val += A[i * _M + k] * B[k * _N + j];
			}
			res[_K * i + j] = curr_val;//A.row(i) * B.col(j);
		}
	}
}

template<typename _T, IndexType _M, IndexType _N, IndexType _K>
Matrix<_T, _M, _K>* parallelMultiply(const Matrix<_T, _M, _N>& A, const Matrix<_T, _N, _K>& B)
{
	Matrix<_T, _M, _K>* res = new Matrix<_T, _M, _K>();
	std::thread threads[CORES];
	auto divs = divisions(B);

	int i = 0;
	std::cout << "Divisions : \n";
	for (auto [left, right] : divs)
	{
		std::cout << "left = " << left << ", right = " << right << ": " << '\n';
		auto lam = [&res, &A, &B, left, right]() { partialMultiply(*res, A, B, left, right); };
		threads[i++] = std::thread(lam);
	}

	for (int i = 0; i < CORES; ++i)
	{
		threads[i].join();
	}

	return res;
}