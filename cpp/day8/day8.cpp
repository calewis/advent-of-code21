#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

struct Line {
  std::array<std::string, 10> left;
  std::array<std::string, 4> right;

  std::vector<std::string> findBySize(int size) const {
    std::vector<std::string> out;
    for (auto const &l : left) {
      if (l.size() == size) {
        out.push_back(l);
      }
    }

    if (out.empty()) {
      fmt::print("Error, no string with size {}\n", size);
      std::abort();
    }

    return out;
  }

  std::string one() const { return findBySize(2).front(); }
  std::string four() const { return findBySize(4).front(); }
  std::string seven() const { return findBySize(3).front(); }
  std::string eight() const { return findBySize(7).front(); }

  char top = 0;
  char mid = 0;
  char bot = 0;
  char ul = 0;
  char ur = 0;
  char ll = 0;
  char lr = 0;
  std::unordered_map<std::string, int> hash;

  friend std::ostream &operator<<(std::ostream &os, Line const &l) {
    for (auto const &s : l.left) {
      os << s << " ";
    }
    os << " | ";
    for (auto const &s : l.right) {
      os << s << " ";
    }
    os << "\n\t";
    os << "t:" << l.top << " ul:" << l.ul << " ll:" << l.ll << " bot:" << l.bot
       << " mid:" << l.mid << " ur:" << l.ur << " lr:" << l.lr;
    return os;
  }

  int getNum(std::string c) {
    std::sort(c.begin(), c.end());
    if (hash.count(c) > 0) {
      return hash[c];
    }
    fmt::print("Error, hashed string not found {}\n", c);
    std::cout << "Line with error: " << *this << "\n";
    std::abort();
  }
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

auto diffSegments(std::string const &s1, std::string const &s2) {
  std::vector<char> diffs;
  for (auto c : s1) {
    auto pos = s2.find(c);
    if (pos == std::string::npos) {
      diffs.push_back(c);
    }
  }

  for (auto c : s2) {
    auto pos = s1.find(c);
    if (pos == std::string::npos) {
      diffs.push_back(c);
    }
  }

  return diffs;
}

auto sameSegments(std::string const &s1, std::string const &s2) {
  std::vector<char> sames;
  for (auto c : s1) {
    auto pos = s2.find(c);
    if (pos != std::string::npos) {
      sames.push_back(c);
    }
  }

  return sames;
}

auto sameSegments(std::vector<std::string> const &vs) {
  std::unordered_map<char, int> counts;

  for (auto const &s : vs) {
    for (auto c : s) {
      if (counts.count(c) > 0) {
        ++counts[c];
      } else {
        counts[c] = 1;
      }
    }
  }

  std::vector<char> out;
  for (auto &[c, count] : counts) {
    if (count == vs.size()) {
      out.push_back(c);
    }
  }

  return out;
}

//
// These functions must be called in order
//
void setTop(Line &l) { l.top = diffSegments(l.seven(), l.one())[0]; }

// Four only has mid, we need to find the value in four that is in both 2 and 5
void setMidAndBot(Line &l) {
  auto fives = l.findBySize(5);
  auto tmb = sameSegments(fives);
  auto four = l.four();
  for (auto c : tmb) {
    if (c == l.top) {
      continue;
    }
    if (four.find(c) != std::string::npos) { // Found mid
      l.mid = c;
    } else {
      l.bot = c;
    }
  }
}

// Of all the sixes only 6 doesn't contain both segments in 1
void setURLRULLL(Line &l) {
  auto sixes = l.findBySize(6);
  auto one = l.one();
  for (auto const &s : sixes) {
    auto diffs = diffSegments(s, one);
    bool is6 = false;
    for (auto c : one) {
      // s was missing a value in one, which means s is 6
      if (std::find(diffs.begin(), diffs.end(), c) != diffs.end()) {
        l.ur = c;
        l.lr = one[one.find_first_not_of(c)];
        is6 = true;
      }
    }

    if (is6) { // Can also figure out upper left with 4
      auto sames = sameSegments(s, l.four());
      for (auto c : sames) {
        if (c != l.mid && c != l.ur && c != l.lr) {
          l.ul = c;
        }
      }

      for (auto c : s) { // Now get the last one
        if (c != l.mid && c != l.ul && c != l.lr && c != l.top && c != l.bot) {
          l.ll = c;
        }
      }
    }
  }
}

template <typename... Cs> std::string fromChars(Cs... c) {
  std::string out;
  (out.push_back(c), ...);
  std::sort(out.begin(), out.end());
  return out;
}

void buildHash(Line &l) {
  l.hash.insert({fromChars(l.ur, l.lr, l.top, l.bot, l.ul, l.ll), 0});
  l.hash.insert({fromChars(l.ur, l.lr), 1});
  l.hash.insert({fromChars(l.top, l.ur, l.mid, l.ll, l.bot), 2});
  l.hash.insert({fromChars(l.top, l.ur, l.mid, l.lr, l.bot), 3});
  l.hash.insert({fromChars(l.ul, l.ur, l.mid, l.lr), 4});
  l.hash.insert({fromChars(l.top, l.ul, l.mid, l.lr, l.bot), 5});
  l.hash.insert({fromChars(l.top, l.ul, l.mid, l.lr, l.ll, l.bot), 6});
  l.hash.insert({fromChars(l.top, l.ur, l.lr), 7});
  l.hash.insert({fromChars(l.top, l.ur, l.lr, l.mid, l.bot, l.ul, l.ll), 8});
  l.hash.insert({fromChars(l.top, l.ur, l.lr, l.mid, l.bot, l.ul), 9});
}

int main(int _, char **argv) {
  auto lines = parseFile(argv[1]);
  auto count = 0UL;
  for (auto const &l : lines) {
    for (auto const &r : l.right) {
      const auto size = r.size();
      if (size == 2 || size == 3 || size == 4 || size == 7) {
        ++count;
      }
    }
  }

  fmt::print("Part 1: {}\n", count);

  auto sum = 0;
  for (auto &l : lines) {
    setTop(l);
    setMidAndBot(l);
    setURLRULLL(l);
    buildHash(l);

    auto num = 0;
    auto pos = 1000;
    for (auto const &r : l.right) {
      auto inum = l.getNum(r);
      num += inum * pos;
      pos /= 10;
    }
    sum += num;
  }
  fmt::print("Part 2: {}\n", sum);
}
