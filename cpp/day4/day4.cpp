#include <Eigen/Dense>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <optional>
#include <unordered_set>

using Mat = Eigen::Matrix<int, 5, 5>;

auto parseRow(std::string const &line) {
  Eigen::Vector<int, 5> out;
  std::stringstream ss(line);
  for (auto i = 0; i < 5; ++i) {
    ss >> out[i];
  }
  return out;
}

auto parseFile(std::string const &fn) {
  auto file = std::ifstream(fn);

  std::string line;
  std::getline(file, line);
  for (auto &c : line) {
    if (c == ',') {
      c = ' ';
    }
  }
  std::stringstream ss(line);

  std::uint16_t num = 0;
  std::vector<std::uint16_t> out;
  while (ss >> num) {
    out.push_back(num);
  }

  // Now for the matrices
  std::vector<Mat> outM;
  int row = 0;
  while (std::getline(file, line)) {
    auto pos = line.find_first_not_of(' ');
    if (pos == std::string::npos || line[pos] == '\n') {
      row = 0;
      outM.emplace_back();
      continue;
    }

    outM.back().row(row++) = parseRow(line);
  }

  return std::make_pair(out, outM);
}

auto matWins(Mat const &m) {
  for (auto r = 0; r < m.rows(); ++r) {
    if (m.row(r).sum() == -5) {
      return true;
    }
  }

  for (auto c = 0; c < m.cols(); ++c) {
    if (m.col(c).sum() == -5) {
      return true;
    }
  }

  return false;
}

int main(int _, char **argv) {
  auto [rands, mats] = parseFile(argv[1]);

  std::optional<Mat> winner;
  std::optional<Mat> loser;
  std::uint16_t rand_winner = 0;
  std::uint16_t rand_loser = 0;
  std::unordered_set<Mat*> winners;
  for (auto r : rands) {
    for (auto &m : mats) {
      if(winners.count(&m) == 1){
        continue;
      }
      m = (m.unaryExpr([&](int x) { return (x == r) ? -1 : x; }));
      if (matWins(m)) {
        if(!winner){
          winner = m;
          rand_winner = r;
        }
        winners.insert(&m);
        if(winners.size() == mats.size()){
          loser = m;
          rand_loser = r;
        }
      }
    }
    if(loser){
      break;
    }
  }

  auto sum = (*winner).unaryExpr([](int x) { return x == -1 ? 0 : x; }).sum();
  auto sum_loser = (*loser).unaryExpr([](int x) { return x == -1 ? 0 : x; }).sum();
  fmt::print("Winning: {}\n", sum * rand_winner);
  fmt::print("Losing: {}\n", sum_loser * rand_loser);
}
