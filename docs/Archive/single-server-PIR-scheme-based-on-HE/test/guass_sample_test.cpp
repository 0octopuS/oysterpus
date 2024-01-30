#include "utils.hpp"
#include <fmt/core.h>
#include <ranges>

int main() {
  std::vector<i64> buckets(256);
  for (auto i : std::views::iota(0, 1000000)) {
    buckets[gaussSample() + 128] += 1;
  }
  for (int i = 0; i < buckets.size(); i++) {
    fmt::print("bucket[{}] == {}\n", i, buckets[i]);
  }
}