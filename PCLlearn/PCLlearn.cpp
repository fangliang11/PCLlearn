#include <iostream>  
#include <Eigen/Dense>  
using namespace Eigen;
int main(int argc, char* argv[])
{
	MatrixXd m(2, 2);
	m(0, 0) = 83;
	m(1, 0) = 10;
	m(0, 1) = -10;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << "Here is the matrix m:\n" << m << std::endl;
	VectorXd v(2);
	v(0) = 4;
	v(1) = v(0) - 1;
	std::cout << "Here is the vector v:\n" << v << std::endl;

	system("pause");
	return 0;
}
