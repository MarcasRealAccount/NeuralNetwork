#include <armadillo>
#include <iostream>

using namespace std;
using namespace arma;

int main(int argc, char** argv)
{
	mat A(4, 5, fill::randu);
	mat B(4, 5, fill::randu);

	cout << A * B.t() << endl;

	return 0;
}