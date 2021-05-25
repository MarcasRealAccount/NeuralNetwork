#pragma once

#include <armadillo>

#include <cstdint>
#include <functional>
#include <ostream>
#include <random>

class Network
{
public:
	using SigmoidFunc = std::function<arma::fvec(const arma::fvec&)>;

private:
	size_t                  numLayers;
	std::vector<size_t>     sizes;
	std::vector<arma::fvec> biases;
	std::vector<arma::fmat> weights;
	SigmoidFunc             sigmoidFunction;
	size_t                  numTest;

	std::mt19937 rng;

public:
	Network(const std::vector<size_t>& sizes, SigmoidFunc sigmoidFunction);

	arma::fvec& feedforward(arma::fvec& a);

	void stochasticGradientDescent(std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t epochs, size_t miniBatchSize, float learningRate, const std::vector<std::pair<arma::fvec, arma::fvec>>& testData = {}, bool clasify = false);

	void updateMiniBatch(const std::vector<std::pair<arma::fvec, arma::fvec>>& trainingData, size_t offset, size_t length, float learningRate);

	void backpropagate(const arma::fvec& input, const arma::fvec& expectedResult, std::vector<arma::fvec>& sgb, std::vector<arma::fmat>& sgw);

	arma::fvec costDerivative(const arma::fvec& result, const arma::fvec& expectedResult);

	arma::fvec sigmoidPrime(const arma::fvec& z) const;

	float  evaluate(const std::vector<std::pair<arma::fvec, arma::fvec>>& testData);
	size_t evaluateClassification(const std::vector<std::pair<arma::fvec, arma::fvec>>& testData);

	friend std::ostream& operator<<(std::ostream& stream, const Network& network);
};

arma::fvec defaultSigmoid(const arma::fvec& z);