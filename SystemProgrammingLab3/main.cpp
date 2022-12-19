#include <iostream>
#include "Matrix.h"
using std::cout;
using std::endl;

int main(void)
{
	Matrix<int, 2, 3> m1 = { 1, 2, 3, 4, 5, 6};
	Matrix<int, 3, 1> m2 = {  7,8,9 };
	cout << m1 << endl;
	cout << m2 << endl;
	cout << (m1 * m2) << endl;

	return 0;
}