#include <fmt/core.h>

#include <fstream>
#include <numeric>
#include <vector>

int main(int argc, char **argv) {
  auto file = std::ifstream(argv[1]);

  std::vector<int> data;
  for (std::string line; std::getline(file, line);) {
    data.push_back(std::stoi(line));
  }

  fmt::print("Part 1 result: {}\n",
             std::transform_reduce(data.begin() + 1, data.end(), data.begin(),
                                   0, std::plus{},
                                   [](int a, int b) { return b < a ? 1 : 0; }));

  fmt::print("Part 2 result: {}\n",
             std::transform_reduce(data.begin() + 3, data.end(), data.begin(),
                                   0, std::plus{},
                                   [](int a, int b) { return b < a ? 1 : 0; }));
}
