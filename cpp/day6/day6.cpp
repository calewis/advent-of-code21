#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
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
  std::vector<int> out;
  int num = 0;
  while(ss >> num){
    out.push_back(num);
  }
  return out;
}

int main(int _, char **argv) {
  auto fish = parseFile(argv[1]);
  fmt::print("Input: {}\n", fmt::join(fish, ","));

  for(auto i = 0; i < 80; ++i){
    auto needs_new = std::count(fish.begin(), fish.end(), 0);
    for(auto &f : fish){
      if(f == 0){
        f = 6;
      } else {
        --f;
      }
    }

    for(auto i = 0; i < needs_new; ++i){
      fish.push_back(8);
    }
  }

  fmt::print("Fish on day 80: {}\n", fish.size());
}
