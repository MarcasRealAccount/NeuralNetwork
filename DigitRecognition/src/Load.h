#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <armadillo>

void loadIDX(const std::string_view& filename, std::vector<std::pair<arma::fvec, arma::fvec>>& inputs);