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
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Mat = Eigen::MatrixXi;

struct Fold {
  enum dir { column, row };

  dir d;
  int coord;
};

Mat linesToMat(std::vector<std::string> &lines) {
  std::vector<std::pair<int, int>> coord;
  int max_x = 0;
  int max_y = 0;
  for (auto &l : lines) {
    if (l.empty()) {
      break;
    }
    for (char &c : l) {
      if (c == ',') {
        c = ' ';
      }
    }

    int x;
    int y;
    std::stringstream ss(l);
    ss >> x;
    ss >> y;

    max_x = std::max(x, max_x);
    max_y = std::max(y, max_y);
    coord.push_back({x, y});
  }

  Mat out(max_y + 1, max_x + 1);
  out.setZero();

  for (auto &[c, r] : coord) {
    out(r, c) = 1;
  }

  return out;
}

std::vector<Fold> linesToFolds(std::vector<std::string> const &lines) {
  std::vector<Fold> folds;
  for (auto const &l : lines) {
    Fold f;
    auto pos = l.find_first_of('=');
    std::string snum(l.data() + pos + 1, l.data() + l.size());
    f.coord = std::stoi(snum);

    auto pos2 = l.find_last_of(' ');
    std::string sdir(l.data() + pos2 + 1, l.data() + pos);

    if (sdir == "x") {
      f.d = Fold::column;
    } else if (sdir == "y") {
      f.d = Fold::row;
    } else {
      fmt::print("Error in fold parsing\n");
      std::abort();
    }

    folds.push_back(f);
  }
  return folds;
}

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<std::string> out;
  while (std::getline(file, line)) {
    out.push_back(line);
  }

  auto mat = linesToMat(out);
  auto blank = std::find_if(out.begin(), out.end(),
                            [](std::string const &l) { return l.empty(); });
  out.erase(out.begin(), blank + 1);
  return std::make_pair(mat, linesToFolds(out));
}

int main(int _, char **argv) {
  auto [M, folds] = parseFile(argv[1]);
  fmt::print("M({}, {})\n", M.rows(), M.cols());
  for (auto const &f : folds) {
    fmt::print("{}: {}\n", f.d, f.coord);
  }

  Mat Mc = M;
  auto sum = 0;
  for (auto const &f : folds) {
    auto coord = f.coord;
    if (f.d == Fold::column) {
      auto r = coord + 1;
      auto l = coord - 1;
      if (coord >= ((Mc.cols() - 1) / 2)) { // Right side has fewer
        while (r < Mc.cols() && l >= 0) {
          Mc.col(l--) += Mc.col(r++);
        }
        Mc = Mc.leftCols(coord).eval();
      } else { // left side has fewer
        while (r < Mc.cols() && l >= 0) {
          Mc.col(r++) += Mc.col(l--);
        }
        Mc = Mc.rightCols(coord).eval();
      }
    } else {
      auto t = coord - 1;
      auto b = coord + 1;
      if (coord >= ((Mc.rows() - 1) / 2)) { // bottom side has fewer
        while (b < Mc.rows() && t >= 0) {
          Mc.row(t--) += Mc.row(b++);
        }
        Mc = Mc.topRows(coord).eval();
      } else { // top side has fewer
        while (b < Mc.rows() && b >= 0) {
          Mc.row(b++) += Mc.row(t--);
        }
        Mc = Mc.bottomRows(coord).eval();
      }
    }

    for (auto i = 0; i < Mc.size(); ++i) {
      if (Mc.data()[i] > 1) {
        Mc.data()[i] = 1;
      }
    }

    if(sum == 0){
      sum = Mc.sum();
    }
  }

  fmt::print("Part 1: {}\n", sum);
  fmt::print("Part 2\n");
  for(auto r = 0; r < Mc.rows(); ++r){
    for(auto c = 0; c < Mc.cols(); ++c){
      if(Mc(r,c) == 1){
        std::cout << "#";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}
