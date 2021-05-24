#include "Network.h"

#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		Network network({ 2, 3, 10 }, defaultSigmoid);
		std::cout << network << std::endl;
		arma::fvec input = { 0.5f, 0.5f };
		std::cout << network.feedforward(input) << std::endl;
		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}