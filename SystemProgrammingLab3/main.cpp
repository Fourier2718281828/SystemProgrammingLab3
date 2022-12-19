#include <iostream>
#include "Matrix.h"
#include "ParallelMultiplication.h"
using std::cout;
using std::endl;

int main(void)
{
	Matrix<int, 2, 23468> m1;
	auto ve = divisions(m1);

	for (auto&& [a, b] : ve)
	{
		cout << "division : [" << a << ", " << b << "];\n";
	}

	return 0;
}