#include "Load.h"
#include "Network.h"

#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		Network network({ 784, 30, 10 }, defaultSigmoid);
		std::cout << network << std::endl;

		std::vector<std::pair<arma::fvec, arma::fvec>> trainingData;
		std::vector<std::pair<arma::fvec, arma::fvec>> testData;
		loadIDX("train", trainingData);
		loadIDX("t10k", testData);

		size_t epochs        = 30;
		size_t miniBatchSize = 10;
		float  learningRate  = 3.0f;

		network.stochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate, testData);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}