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

bool isLower(std::string const &str) {
  std::string criteria("abcdefghijklmnopqrstuvwxyz");
  return (std::string::npos == str.find_first_not_of(criteria));
}

class Node {
  std::string name_;
  std::unordered_set<Node *> neighbors_;

public:
  Node() = default;
  Node(std::string name) : name_(std::move(name)) {}

  void addNeighbor(Node *n) { neighbors_.insert(n); }
  std::unordered_set<Node *> const &neighbors() { return neighbors_; }
  std::string const &name() const { return name_; }
  bool isLower() const { return ::isLower(name_); }
};

Node *linesToNodes(std::vector<std::string> const &lines) {
  std::unordered_map<std::string, Node *> nodes;
  for (auto const &l : lines) {
    auto pos = l.find_first_of('-');
    std::string left_name(l.data(), l.data() + pos);
    std::string right_name(l.data() + pos + 1, l.data() + l.size());

    auto lit = nodes.find(left_name);
    Node *left = nullptr;
    if (lit == nodes.end()) {
      left = new Node(left_name);
      nodes.insert({left_name, left});
    } else {
      left = lit->second;
    }

    auto rit = nodes.find(right_name);
    Node *right = nullptr;
    if (rit == nodes.end()) {
      right = new Node(right_name);
      nodes.insert({right_name, right});
    } else {
      right = rit->second;
    }

    left->addNeighbor(right);
    right->addNeighbor(left);
  }

  return nodes["start"];
}

auto parseFile(char const *file_name) {
  auto file = std::ifstream(file_name);
  std::string line;
  std::vector<std::string> out;
  while (std::getline(file, line)) {
    out.push_back(line);
  }

  return linesToNodes(out);
}

std::size_t recDFS(Node *n, std::unordered_set<Node *> seen, bool seen_small) {
  if (n->name() == "end") {
    return 1;
  } 

  if (n->isLower()) {
    seen.insert(n);
  }

  std::size_t num_paths = 0;
  for (auto *neigh : n->neighbors()) {
    if(neigh->name() == "start"){
      continue;
    }

    if (seen.count(neigh) == 0) {
      num_paths += recDFS(neigh, seen, seen_small);
    } else if(!seen_small){
      num_paths += recDFS(neigh, seen, true);
    }
  }

  return num_paths;
}

int main(int _, char **argv) {
  auto *start = parseFile(argv[1]);
  fmt::print("Part1 result: {}\n", recDFS(start, {start}, true));
  fmt::print("Part2 result: {}\n", recDFS(start, {start}, false));
}
