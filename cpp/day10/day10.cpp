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
#include <vector>

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<std::string> out;
  while (std::getline(file, line)) {
    out.push_back(line);
  }

  return out;
}

std::optional<char> corrupted(std::string_view v) {
  std::stack<char> s;
  for (auto c : v) {
    switch (c) {
    case '{':
      s.push('}');
      break;
    case '<':
      s.push('>');
      break;
    case '[':
      s.push(']');
      break;
    case '(':
      s.push(')');
      break;
    case '}':
    case '>':
    case ']':
    case ')':
      if (s.top() != c) {
        return c;
      }
      s.pop();
      break;
    default:
      fmt::print("char {} not allowed.\n", c);
      std::abort();
    }
  }

  return {};
}

auto completeAndScore(std::string_view v) {
  std::stack<char> s;
  for (auto c : v) {
    switch (c) {
    case '{':
      s.push('}');
      break;
    case '<':
      s.push('>');
      break;
    case '[':
      s.push(']');
      break;
    case '(':
      s.push(')');
      break;
    case '}':
    case '>':
    case ']':
    case ')':
      if (s.top() != c) {
        fmt::print("Corrupted!\n");
        std::abort();
      }
      s.pop();
      break;
    default:
      fmt::print("char {} not allowed.\n", c);
      std::abort();
    }
  }

  auto score = 0UL;
  while(!s.empty()){
    score *= 5;
    switch(s.top()){
      case ']':
        score += 2;
        break;
      case ')':
        score += 1;
        break;
      case '}':
        score += 3;
        break;
      case '>':
        score += 4;
        break;
    }
    s.pop();
  }

  return score;
}

int main(int _, char **argv) {
  auto lines = parseFile(argv[1]);

  std::vector<char> mismatches;
  std::vector<std::size_t> scores;
  for (auto const &l : lines) {
    if (auto bad = corrupted(l)) {
      mismatches.push_back(*bad);
    } else {
      scores.push_back(completeAndScore(l));
    }
  }

  auto sum = 0UL;
  for (auto m : mismatches) {
    switch (m) {
    case '}':
      sum += 1197;
      break;
    case ')':
      sum += 3;
      break;
    case ']':
      sum += 57;
      break;
    case '>':
      sum += 25137;
      break;
    }
  }

  fmt::print("Part 1 score: {}\n", sum);

  std::sort(scores.begin(), scores.end());
  for(auto i = 0; i < scores.size(); ++i){
    fmt::print("{}: {}\n", i, scores[i]);
  }
  fmt::print("Part 2 score: {}\n", scores[scores.size()/2]);
}
