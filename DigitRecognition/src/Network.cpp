#include "Network.h"

#include <algorithm>
#include <random>
#include <sstream>

Network::Network(const std::vector<size_t>& sizes, SigmoidFunc sigmoidFunction)
    : numLayers(sizes.size()), sizes(sizes), biases(sizes.size() - 1), weights(sizes.size() - 1), sigmoidFunction(sigmoidFunction), numTest(0)
{
	arma::arma_rng::set_seed(rng());
	for (size_t i = 1; i < sizes.size(); i++)
	{
		biases[i - 1]  = arma::fvec(sizes[i], arma::fill::randn);
		weights[i - 1] = arma::fmat(sizes[i], sizes[i - 1], arma::fill::randn);
	}
}

arma::fvec& Network::feedforward(arma::fvec& a)
{
	for (size_t i = 0; i < numLayers - 1; i++)
		a = sigmoidFunction(weights[i] * a + biases[i]);
	return a;
}

void Network::stochasticGradientDescent(std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t epochs, size_t miniBatchSize, float learningRate, const std::vector<std::pair<arma::fvec, arma::fvec>>& testData, bool clasify)
{
	if (!testData.empty())
		numTest = testData.size();

	size_t n = trainingData.size();
	for (size_t j = 0; j < epochs; j++)
	{
		std::shuffle(trainingData.begin(), trainingData.end(), rng);

		for (size_t i = 0; i < n; i += miniBatchSize)
			updateMiniBatch(trainingData, i, miniBatchSize, learningRate);

		std::ostringstream str;
		if (!testData.empty())
		{
			if (clasify)
				str << "Epoch " << j << ": " << evaluateClassification(testData) << " / " << numTest << "\n";
			else
				str << "Epoch " << j << ": " << (evaluate(testData) * 100) << "% of " << numTest << " tests\n";
		}
		else
		{
			str << "Epoch " << j << ": complete\n";
		}
		std::cout << str.str();
	}
}

void Network::updateMiniBatch(const std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t offset, size_t length, float learningRate)
{
	std::vector<arma::fvec> sgb(numLayers - 1);
	std::vector<arma::fmat> sgw(numLayers - 1);
	for (size_t i = 0; i < numLayers - 1; i++)
	{
		sgb[i] = arma::fvec(biases[i].n_rows, arma::fill::zeros);
		sgw[i] = arma::fmat(weights[i].n_rows, weights[i].n_cols, arma::fill::zeros);
	}

	size_t usedLength = std::min(length, trainingData.size() - offset);
	for (size_t i = offset; i < offset + usedLength; i++)
	{
		auto& [x, y] = trainingData[i];
		backpropagate(x, y, sgb, sgw);
	}

	float learningFactor = learningRate / usedLength;
	for (size_t i = 0; i < numLayers - 1; i++)
	{
		biases[i] -= learningFactor * sgb[i];
		weights[i] -= learningFactor * sgw[i];
	}
}

template <class T>
auto& indexNegativeAllowed(const T& t, ptrdiff_t index)
{
	if (index < 0)
		return t[t.size() + index];
	else
		return t[index];
}

template <class T>
auto& indexNegativeAllowed(T& t, ptrdiff_t index)
{
	if (index < 0)
		return t[t.size() + index];
	else
		return t[index];
}

void Network::backpropagate(const arma::fvec& input, const arma::fvec& expectedResult, std::vector<arma::fvec>& sgb, std::vector<arma::fmat>& sgw)
{
	arma::fvec              activation = input;
	std::vector<arma::fvec> activations(numLayers);
	activations[0] = input;
	std::vector<arma::fvec> zs(numLayers - 1);
	for (size_t i = 0; i < numLayers - 1; i++)
		activations[i + 1] = activation = sigmoidFunction(zs[i] = weights[i] * activation + biases[i]);

	arma::fvec delta = costDerivative(activations[activations.size() - 1], expectedResult) % sigmoidPrime(zs[zs.size() - 1]);
	sgb[sgb.size() - 1] += delta;
	sgw[sgw.size() - 1] += delta * activations[activations.size() - 2].t();

	for (size_t l = 2; l < numLayers; l++)
	{
		delta = (indexNegativeAllowed(weights, -l + 1).t() * delta) % sigmoidPrime(indexNegativeAllowed(zs, -l));
		indexNegativeAllowed(sgb, -l) += delta;
		indexNegativeAllowed(sgw, -l) += delta * indexNegativeAllowed(activations, -l - 1).t();
	}
}

arma::fvec Network::costDerivative(const arma::fvec& result, const arma::fvec& expectedResult)
{
	return result - expectedResult;
}

arma::fvec Network::sigmoidPrime(const arma::fvec& z) const
{
	return sigmoidFunction(z) % (1 - sigmoidFunction(z));
}

float Network::evaluate(const std::vector<std::pair<arma::fvec, arma::fvec>>& testData)
{
	float totalError = 0.0f;
	for (auto& [x, y] : testData)
	{
		float      error = 0.0f;
		arma::fvec input = x;
		feedforward(input);
		for (size_t i = 0; i < input.n_rows; i++)
			error += std::abs(y[i] - input[i]);
		error /= input.n_rows;
		totalError += error;
	}
	return 1.0f - (totalError / testData.size());
}

size_t Network::evaluateClassification(const std::vector<std::pair<arma::fvec, arma::fvec>>& testData)
{
	size_t sum = 0;
	for (auto& [x, y] : testData)
	{
		arma::fvec input = x;
		feedforward(input);

		size_t largestResultIndex         = -1;
		size_t largestExpectedResultIndex = -1;
		float  largestResult              = -std::numeric_limits<float>::max();
		float  largestExpectedResult      = -std::numeric_limits<float>::max();
		for (size_t i = 0; i < input.n_rows; i++)
		{
			float result         = input[i];
			float expectedResult = y[i];
			if (result > largestResult)
			{
				largestResult      = result;
				largestResultIndex = i;
			}
			if (expectedResult > largestExpectedResult)
			{
				largestExpectedResult      = expectedResult;
				largestExpectedResultIndex = i;
			}
		}
		sum += largestResultIndex == largestExpectedResultIndex;
	}
	return sum;
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