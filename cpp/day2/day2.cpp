#include <fmt/core.h>

#include <fstream>
#include <sstream>
#include <vector>

struct Inst {
  Inst(std::string const &line) {
    std::stringstream ss(line);
    ss >> c;
    ss >> val;
  }

  std::string c;
  int val = 0;
};

int main(int argc, char **argv) {
  auto file = std::ifstream(argv[1]);

  std::vector<Inst> data;
  for (std::string line; std::getline(file, line);) {
    data.push_back(Inst(line));
  }

  int aim = 0;
  int depth = 0;
  int disp = 0;
  for (auto &[str, val] : data) {
    switch (str[0]) {
    case 'f':
      disp += val;
      depth += aim * val;
      break;
    case 'd':
      aim += val;
      break;
    case 'u':
      aim -= val;
      break;
    default:
      fmt::print("Parsing failed\n");
      std::terminate();
    }
  }

  fmt::print("Part 1 {}, Part 2 {}\n", aim * disp, depth * disp);
}
