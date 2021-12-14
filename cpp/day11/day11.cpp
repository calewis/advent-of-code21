#include <Eigen/Dense>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using Mat = Eigen::MatrixXi;

auto makeMat(std::vector<std::string> const &lines) {
  Mat m(10, 10);
  m.setZero();

  for (auto i = 0; i < lines.size(); ++i) {
    auto row = lines[i];
    for (auto j = 0; j < row.size(); ++j) {
      std::string val;
      val += row[j];
      m(i, j) = std::stoi(val);
    }
  }

  return m;
}

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<std::string> out;
  while (std::getline(file, line)) {
    out.push_back(line);
  }

  return makeMat(out);
}

std::size_t absorb(Mat &m, int i, int j) {
  if (i < 0 || i >= m.rows() || j < 0 || j >= m.cols() || m(i, j) == 0) {
    return 0; // Don't absorb if we don't exist or already flashed
  }

  m(i, j) += 1;
  std::size_t out = 0;
  if (m(i, j) > 9) { // Flash
    m(i, j) = 0;
    out += 1; // Count our flash
    out += absorb(m, i + 1, j);
    out += absorb(m, i - 1, j);
    out += absorb(m, i, j + 1);
    out += absorb(m, i, j - 1);
    out += absorb(m, i + 1, j + 1);
    out += absorb(m, i - 1, j + 1);
    out += absorb(m, i - 1, j - 1);
    out += absorb(m, i + 1, j - 1);
  }

  return out;
}

std::size_t step(Mat &m) {
  // Increase energy levels by one
  for (auto r = 0; r < m.rows(); ++r) {
    for (auto c = 0; c < m.cols(); ++c) {
      m(r, c) += 1;
    }
  }

  std::size_t out = 0;
  for (auto r = 0; r < m.rows(); ++r) {
    for (auto c = 0; c < m.cols(); ++c) {
      if (m(r, c) > 9) {
        m(r, c) = 0;
        out += 1;
        out += absorb(m, r + 1, c);
        out += absorb(m, r - 1, c);
        out += absorb(m, r, c + 1);
        out += absorb(m, r, c - 1);
        out += absorb(m, r + 1, c + 1);
        out += absorb(m, r - 1, c + 1);
        out += absorb(m, r - 1, c - 1);
        out += absorb(m, r + 1, c - 1);
      }
    }
  }

  return out;
}

int main(int _, char **argv) {
  auto M = parseFile(argv[1]);
  std::cout << "M:\n" << M << "\n\n";

  auto Mp1 = M;
  std::size_t sum = 0;
  for(auto i = 0; i < 100; ++i){
    sum += step(Mp1);
  }
  fmt::print("Part1 score: {}\n", sum);

  auto day = 0;
  while(M.sum() != 0){
    ++day;
    step(M);
  }
  fmt::print("Part2 day: {}\n", day);
}
