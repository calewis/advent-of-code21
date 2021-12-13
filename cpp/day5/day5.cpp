#include <Eigen/Dense>
#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct Line {
  std::array<int, 2> x = {};
  std::array<int, 2> y = {};
};

std::ostream &operator<<(std::ostream &os, Line const &l) {
  os << "[(" << l.x[0] << "," << l.x[1] << "),(" << l.y[0] << "," << l.y[1]
     << ")]";
  return os;
}

Line parseLine(std::string_view v) {
  auto first_comma = v.find_first_of(',');
  std::string first_int(v.data(), v.data() + first_comma);
  Line l;
  l.x[0] = std::stoi(first_int);

  auto first_space = v.find_first_of(' ');
  std::string second_int(v.data() + first_comma + 1, first_space);
  l.y[0] = std::stoi(second_int);

  auto last_space = v.find_last_of(' ');
  auto last_comma = v.find_last_of(',');
  std::string third_int(v.data() + last_space + 1, v.data() + last_comma);
  l.x[1] = std::stoi(third_int);

  std::string fourth_int(v.data() + last_comma + 1, v.data() + v.size());
  l.y[1] = std::stoi(fourth_int);

  return l;
}

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<Line> out;
  while (std::getline(file, line)) {
    out.push_back(parseLine(line));
  }

  return out;
}

auto maxExtent(std::vector<Line> const &ls) {
  auto const &l =
      *std::max_element(ls.begin(), ls.end(), [](Line const &a, Line const &b) {
        return std::max({a.x[0], a.x[1], a.y[0], a.y[1]}) <
               std::max({b.x[0], b.x[1], b.y[0], b.y[1]});
      });
  return std::max({l.x[0], l.x[1], l.y[0], l.y[1]});
}

int main(int _, char **argv) {
  auto input = parseFile(argv[1]);
  auto max_extent = maxExtent(input);
  fmt::print("Max extent {}\n", max_extent);
  Eigen::MatrixXi M(max_extent + 1, max_extent + 1);
  M.setZero();

  for (auto const &l : input) {
    if (l.x[0] != l.x[1] && l.y[0] != l.y[1]) {
      continue;
    }

    if (l.x[0] == l.x[1]) {
      auto line = l.y;
      std::sort(line.begin(), line.end());
      auto col = M.col(l.x[0]);
      for (auto i = line[0]; i <= line[1]; ++i) {
        ++col(i);
      }
    } else {
      auto line = l.x;
      std::sort(line.begin(), line.end());
      auto row = M.row(l.y[0]);
      for (auto i = line[0]; i <= line[1]; ++i) {
        ++row(i);
      }
    }
  }

  auto count = 0;
  for (auto i = 0; i < M.size(); ++i) {
    if (M.data()[i] > 1) {
      ++count;
    }
  }

  fmt::print("Num overlap {}\n", count);
}
