#include "Network.h"

#include <algorithm>
#include <random>
#include <sstream>

Network::Network(const std::vector<size_t>& sizes, SigmoidFunc sigmoidFunction)
    : numLayers(sizes.size()), sizes(sizes), biases(sizes.size() - 1), weights(sizes.size() - 1), sigmoidFunction(sigmoidFunction)
{
	for (size_t i = 1; i < sizes.size(); i++)
	{
		biases[i - 1]  = arma::fmat(sizes[i], 1, arma::fill::randn);
		weights[i - 1] = arma::fmat(sizes[i], sizes[i - 1], arma::fill::randn);
	}
}

arma::fmat& Network::feedforward(arma::fmat& a)
{
	for (size_t i = 0; i < numLayers - 1; i++)
	{
		arma::fmat& b = biases[i];
		arma::fmat& w = weights[i];

		a = sigmoidFunction(w * a + b);
	}
	return a;
}

void Network::stochasticGradientDescent(std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t epochs, size_t miniBatchSize, float learningRate, const std::vector<std::pair<arma::fvec, arma::fvec>>& testData)
{
	if (!testData.empty())
	{
	}

	size_t n = trainingData.size();
	for (size_t j = 0; j < epochs; j++)
	{
		std::shuffle(trainingData.begin(), trainingData.end(), std::mt19937_64 {});
	}
}

arma::fvec defaultSigmoid(const arma::fvec& z)
{
	return 1.0f / (1.0f + arma::exp(-z));
}

std::ostream& operator<<(std::ostream& stream, const Network& network)
{
	std::ostringstream str;
	str << "Network (" << network.numLayers << ")" << std::endl;
	str << "Sizes: { ";
	for (size_t i = 0; i < network.sizes.size(); i++)
	{
		if (i > 0)
			str << ", ";
		str << network.sizes[i];
	}
	str << " }" << std::endl;

#ifdef _DEBUG
	size_t totalBiasSize = 0;
	for (auto& bias : network.biases)
		totalBiasSize += bias.n_rows;

	if (totalBiasSize < 50)
	{
		str << "Biases: {" << std::endl;
		for (size_t i = 0; i < network.biases.size(); i++)
		{
			if (i > 0)
				str << std::endl;
			str << network.biases[i];
		}
		str << " }" << std::endl;
	}
	else
	{
		str << "Biases: " << network.biases.size() << std::endl;
	}
#else
	str << "Biases: " << network.biases.size() << std::endl;
#endif

#ifdef _DEBUG
	size_t totalWeightSize = 0;
	for (auto& weight : network.weights)
		totalWeightSize += weight.n_rows * weight.n_rows;

	if (totalWeightSize < 50)
	{
		str << "Weights: {" << std::endl;
		for (size_t i = 0; i < network.weights.size(); i++)
		{
			if (i > 0)
				str << std::endl;
			str << network.weights[i];
		}
		str << " }";
	}
	else
	{
		str << "Weights: " << network.weights.size() << std::endl;
	}
#else
	str << "Weights: " << network.weights.size() << std::endl;
#endif

	return stream << str.str();
}