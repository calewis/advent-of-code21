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
  std::getline(file, line);

  std::transform(line.begin(), line.end(), line.begin(),
                 [](char &c) { return c == ',' ? ' ' : c; });

  std::stringstream ss(line);
  std::vector<std::uint8_t> out;
  int num = 0;
  while (ss >> num) {
    out.push_back(num);
  }
  return out;
}

int main(int _, char **argv) {
  auto fish = parseFile(argv[1]);
  fmt::print("Input: {}\n", fmt::join(fish, ","));

  std::array<std::size_t, 9> fish_counts = {};
  for (auto f : fish) {
    ++fish_counts[f];
  }

  fmt::print("Fish counts on day 0: {}\n", fmt::join(fish_counts, ","));

  for (auto i = 0; i < 256; ++i) {
    auto new_fish = fish_counts[0];
    for (auto i = 1; i < 9; ++i) {
      fish_counts[i - 1] = fish_counts[i];
    }
    fish_counts[6] += new_fish;
    fish_counts[8] = new_fish;

    auto sum = std::reduce(fish_counts.begin(), fish_counts.end(), 0UL);
    fmt::print("Fish on day {}: {}\n", i + 1, sum);
  }
}
