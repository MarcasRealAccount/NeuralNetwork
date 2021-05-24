#pragma once

#include <armadillo>

#include <cstdint>
#include <functional>
#include <ostream>

class Network
{
public:
	using SigmoidFunc = std::function<arma::fvec(const arma::fvec&)>;

private:
	size_t                  numLayers;
	std::vector<size_t>     sizes;
	std::vector<arma::fmat> biases;
	std::vector<arma::fmat> weights;
	SigmoidFunc             sigmoidFunction;
	size_t                  numTest;

public:
	Network(const std::vector<size_t>& sizes, SigmoidFunc sigmoidFunction);

	arma::fmat& feedforward(arma::fmat& a);

	void stochasticGradientDescent(std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t epochs, size_t miniBatchSize, float learningRate, const std::vector<std::pair<arma::fvec, arma::fvec>>& testData = {});

	void updateMiniBatch(const std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t offset, size_t length, float learningRate);

	std::pair<std::vector<arma::fmat>, std::vector<arma::fmat>> backpropagate(const arma::fvec& input, const arma::fvec& expectedResult);

	size_t evaluate(const std::vector<std::pair<arma::fvec, arma::fvec>>& testData);

	friend std::ostream& operator<<(std::ostream& stream, const Network& network);
};

arma::fvec defaultSigmoid(const arma::fvec& z);