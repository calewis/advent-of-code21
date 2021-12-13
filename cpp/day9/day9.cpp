#include <Eigen/Dense>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<std::vector<int>> temp;
  while (std::getline(file, line)) {
    std::vector<int> t;
    // Not sure why passing chars to stoi didn't work but what evers
    // Probably needs null termination
    for (auto i = 0; i < line.size(); ++i) {
      std::string c = {line[i]};
      if (c != "\n") {
        t.push_back(std::stoi(c));
      }
    }
    temp.push_back(std::move(t));
  }

  auto rows = temp.size();
  auto cols = temp[0].size();

  Eigen::MatrixXi out(rows, cols);
  out.setZero();

  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < cols; ++j) {
      out(i, j) = temp[i][j];
    }
  }

  return out;
}

auto findLow(auto const &m) {
  Eigen::MatrixXi out(m.rows(), m.cols());
  out.setConstant(-1);

  for (auto r = 0; r < m.rows(); ++r) {
    for (auto c = 0; c < m.cols(); ++c) {
      if (r == 0 && c == 0) {
        if (m(0, 0) < m(0, 1) && m(0, 0) < m(1, 0)) { // upper left
          out(r, c) = m(r, c);
        }
      } else if (r == (m.rows() - 1) && c == (m.cols() - 1)) { // bottom right
        if (m(r, c) < m(r - 1, c) && m(r, c) < m(r, c - 1)) {
          out(r, c) = m(r, c);
        }
      } else if (r == 0 && c == (m.cols() - 1)) { // upper right
        if (m(r, c) < m(r + 1, c) && m(r, c) < m(r, c - 1)) {
          out(r, c) = m(r, c);
        }
      } else if (r == (m.rows() - 1) && c == 0) { // lower left
        if (m(r, c) < m(r - 1, c) && m(r, c) < m(r, c + 1)) {
          out(r, c) = m(r, c);
        }
      } else if (r == 0) { // top row
        if (m(r, c) < m(r + 1, c) && m(r, c) < m(r, c + 1) &&
            m(r, c) < m(r, c - 1)) {
          out(r, c) = m(r, c);
        }
      } else if (r == (m.rows() - 1)) { // bottom row
        if (m(r, c) < m(r - 1, c) && m(r, c) < m(r, c + 1) &&
            m(r, c) < m(r, c - 1)) {
          out(r, c) = m(r, c);
        }
      } else if (c == 0) { // Left col
        if (m(r, c) < m(r, c + 1) && m(r, c) < m(r + 1, c) &&
            m(r, c) < m(r - 1, c)) {
          out(r, c) = m(r, c);
        }
      } else if (c == (m.cols() - 1)) { // right col
        if (m(r, c) < m(r, c - 1) && m(r, c) < m(r + 1, c) &&
            m(r, c) < m(r - 1, c)) {
          out(r, c) = m(r, c);
        }
      } else { // Whew in the middle
        if (m(r, c) < m(r, c + 1) && m(r, c) < m(r, c - 1) &&
            m(r, c) < m(r + 1, c) && m(r, c) < m(r - 1, c)) {
          out(r, c) = m(r, c);
        }
      }
    }
  }

  return out;
}

int main(int _, char **argv) {
  auto M = parseFile(argv[1]);
  if (M.rows() < 11 && M.cols() < 11) {
    std::cout << "M:\n" << M << "\n\n";
  }
  auto lows = findLow(M);
  if (M.rows() < 11 && M.cols() < 11) {
    std::cout << "L:\n" << lows << "\n\n";
  }
  auto sum = 0;
  for (auto i = 0; i < lows.rows(); ++i) {
    for (auto j = 0; j < lows.cols(); ++j) {
      if (lows(i, j) != -1) {
        sum += lows(i, j) + 1;
      }
    }
  }

  fmt::print("Sum of low points: {}\n", sum);
}
