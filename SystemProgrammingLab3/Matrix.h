#pragma once
#include <array>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <cassert>
#include <iostream>
#include "IncorrectInputException.h"

using IndexType = size_t;

template<typename _T, IndexType _M, IndexType _N>
class Matrix
{
public:
	static_assert(_M > 0 && _N > 0, "Matrix dimensions cannot be non-positive.");
public:
	using container_type = std::array<_T, _M * _N>;
	using row_iterator = container_type::iterator;
	using const_row_iterator = container_type::const_iterator;
public:
	Matrix() = default;

	template<std::convertible_to<_T> _U>
	Matrix(std::initializer_list<_U> list)
	{
		IndexType i = 0u; 
		auto it = list.begin();

		for (; i < _array.size() && it != list.end(); ++i, ++it)
		{
			_array[i] = static_cast<_T>(*it);
		}

		if (i < _array.size() || it != list.end())
			throw IncorrectInputException("Initializer list input cannot fit into matrix container.");
	}

	template<std::convertible_to<_T> _U>
	Matrix(const Matrix<_U, _M, _N>& m)
	{
		auto it = m.row_cbegin();
		for (auto&& elem : _array)
		{
			elem = *it++;
		}
	}


	~Matrix() = default;

public:

	Matrix& operator=(const Matrix&)& = default;

	Matrix& operator+= (const Matrix& A)&
	{
		auto it1 = _array.begin();
		auto it2 = A.row_cbegin();

		for (; it1 != _array.end(); ++it1, ++it2)
		{
			*it1 += static_cast<_T>(*it2);
		}

		assert(it2 == A.row_cend());

		return *this;
	}

public:

	constexpr IndexType size() const noexcept
	{
		return _array.size();
	}

	constexpr IndexType M() const noexcept
	{
		return _M;
	}

	constexpr IndexType N() const noexcept
	{
		return _N;
	}

	const _T& operator[] (const IndexType i) const
	{
		return _array[i];
	}

	_T& operator[](const IndexType i)
	{
		return _array[i];
	}

	row_iterator row_begin()
	{
		return _array.begin();
	}

	const_row_iterator row_cbegin() const
	{
		return _array.cbegin();
	}

	row_iterator row_end()
	{
		return _array.end();
	}

	const_row_iterator row_cend() const
	{
		return _array.cend();
	}

	const Matrix<_T, 1, _N> row(IndexType i) const
	{
		if (i < 0 || i >= _M)
			throw IncorrectInputException("Incorrect column index.");
		Matrix<_T, 1, _N> res;
		auto it = res.row_begin();
		for (IndexType j = _N * i; j < (i + 1) * _N; ++j, ++it)
		{
			*it = _array[j];
		}
		return res;
	}

	const Matrix<_T, _M, 1> col(IndexType j) const
	{
		if (j < 0 || j >= _N)
			throw IncorrectInputException("Incorrect column index.");
		Matrix<_T, _M, 1 > res;
		IndexType r = 0;
		for (IndexType i = j; i < size(); i += _N, ++r)
		{
			res[r] = _array[i];
			std::cout << "index: " << i << '\n';
		}
		
		return res;
	}

private:
	container_type _array;
};

template<typename _T, IndexType _M, IndexType _N>
Matrix<_T, _M, _N> operator+ (const Matrix<_T, _M, _N>& A, const Matrix<_T, _M, _N>& B)
{
	Matrix res(A);
	return res += B;
}

template<typename _T, IndexType _M, IndexType _N>
std::ostream& operator<<(std::ostream& o, const Matrix<_T, _M, _N>& m)
{
	for (IndexType i = 0u; i < _M; ++i)
	{
		for (IndexType j = 0u; j < _N; ++j)
		{
			o << m[_N * i + j] << ' ';
		}
		
		o << '\n';
	}

	return o;
}

template<typename _T, IndexType _M, IndexType _N, IndexType _K>
Matrix<_T, _M, _K> operator* (const Matrix<_T, _M, _N>& A, const Matrix<_T, _N, _K>& B)
{
	Matrix<_T, _M, _K> res;
	for (IndexType i = 0; i < _M; ++i)
	{
		for (IndexType j = 0; j < _K; ++j)
		{
			res[_K * i + j] = A.row(i) * B.col(j);
		}
	}

	return res;
}

template<typename _T, IndexType _M>
_T operator* (const Matrix<_T, 1, _M>& A, const Matrix<_T, _M, 1>& B)
{
	_T res = 0;
	for (IndexType i = 0u; i < _M; ++i)
	{
		res += A[i] * B[i];
	}
	return res;
}