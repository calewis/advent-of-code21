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

struct Line {
  std::array<std::string, 10> left;
  std::array<std::string, 4> right;
};

auto parseLine(std::string const &line) {
  std::stringstream ss(line);
  std::string word;
  Line l;
  for (auto i = 0; i < 10; ++i) {
    ss >> word;
    l.left[i] = word;
  }
  ss >> word; // '|'
  for (auto i = 0; i < 4; ++i) {
    ss >> word;
    l.right[i] = word;
  }

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

int main(int _, char **argv) {
  auto lines = parseFile(argv[1]);
  auto count = 0UL;
  for(auto const &l : lines){
    for(auto const &r : l.right){
      const auto size = r.size();
      if(size == 2 || size == 3 || size == 4 || size == 7){
        ++count;
      }
    }
  }

  fmt::print("Part 1: {}\n", count);

}
