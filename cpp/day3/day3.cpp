#include <array>
#include <cstdint>
#include <fmt/core.h>
#include <fstream>
#include <string>
#include <vector>

constexpr std::size_t data_width = 12;

auto parseFile(std::string const &fn) {
  auto file = std::ifstream(fn);
  std::string line;
  std::vector<std::uint16_t> out;
  while (std::getline(file, line)) {
    out.push_back(std::stoul(line, nullptr, 2));
  }

  return out;
}

int main(int _, char **argv) {
  auto data = parseFile(argv[1]);

  std::array<std::size_t, data_width> count = {};
  for (auto d : data) {
    for (auto i = 0; i < data_width; ++i) {
      count[i] += (d & (1 << i)) > 0 ? 1 : 0;
    }
  }

  auto m_thresh = data.size() / 2;
  for (auto &c : count) {
    c /= m_thresh;
  }

  std::uint16_t gamma = 0;
  for (auto i = 0; i < data_width; ++i) {
    gamma += count[i] << i;
  }

  auto epsilon = gamma ^ 0xFFF;
  fmt::print("Part 1 Power {}\n", epsilon * gamma);

  auto oxy = data;
  auto o2r = 0UL;
  for (auto i = 0; i < data_width; ++i) {
    auto zeros = std::partition(oxy.begin(), oxy.end(), [i = i](auto n) {
      return (n & (1 << (11 - i))) > 0;
    });

    auto len1 = std::distance(oxy.begin(), zeros);
    auto len2 = std::distance(zeros, oxy.end());

    if (len1 >= len2) {
      oxy.erase(zeros, oxy.end());
    } else {
      oxy.erase(oxy.begin(), zeros);
    }

    if (oxy.size() == 1) {
      o2r = oxy[0];
      break;
    }
  }

  oxy = data;
  auto co2r = 0UL;
  for (auto i = 0; i < data_width; ++i) {
    auto zeros = std::partition(oxy.begin(), oxy.end(), [i = i](auto n) {
      return (n & (1 << (11 - i))) > 0;
    });

    auto len1 = std::distance(oxy.begin(), zeros);
    auto len2 = std::distance(zeros, oxy.end());

    if (len2 <= len1) {
      oxy.erase(oxy.begin(), zeros);
    } else {
      oxy.erase(zeros, oxy.end());
    }

    if (oxy.size() == 1) {
      co2r = oxy[0];
      break;
    }
  }

  fmt::print("o2: {}, co2: {}\n", o2r, co2r);
  fmt::print("Part 2: {}\n", o2r * co2r);
  return 0;
}
