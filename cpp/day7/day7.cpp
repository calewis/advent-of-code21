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
  std::vector<std::uint16_t> out;
  int num = 0;
  while (ss >> num) {
    out.push_back(num);
  }
  return out;
}

int main(int _, char **argv) {
  auto crabs = parseFile(argv[1]);
  fmt::print("Input: {}\n", fmt::join(crabs, ", "));

  // Is this the bounds?
  auto [min_it, max_it] = std::minmax_element(crabs.begin(), crabs.end());

  auto best = std::numeric_limits<std::size_t>::max();
  for(auto i = *min_it; i <= *max_it; ++i){
    auto score = 0UL;
    for(auto c : crabs){
      if(c > i){
        score  += c - i;
      } else {
        score += i - c;
      }
    }
    best = std::min(best, score);
  }

  fmt::print("Best fuel usage: {}\n", best);
}
