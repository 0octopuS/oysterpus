#include "database.hpp"
#include "pir.hpp"
#include "simplePirServer.hpp"
#include "utils.hpp"
#include <fmt/core.h>
#include <ranges>
#include <vector>

#define LOGQ u64(32)
#define SEC_PARAM u64(1 << 10)

void test_DB_Medium_elements() {
  fmt::println("\n>>>> Medium emlements");
  u64 N = 4, d = 9;
  std::vector<u64> vals{1, 2, 3, 4};
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);
  Database DB{N, d, &p, vals};
  if (DB.Info.Elems_per_ptxt != 1 || DB.Info.Ptxt_per_elems != 1) {
    fmt::println("Setup Bad");
  } else {
    fmt::println("Setup Good");
  }
  for (u64 i = 0; i < N; ++i) {
    if (DB.getElem(i) != (i + 1)) {
      fmt::println("Get Bad: {} != {}", DB.getElem(i), i + 1);
    }
  }
}

void test_DB_Small_elements() {
  fmt::println("\n>>>> Small emlements");
  u64 N = 4, d = 3;
  std::vector<u64> vals{1, 2, 3, 4};
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);
  Database DB{N, d, &p, vals};

  if (DB.Info.Elems_per_ptxt <= 1 || DB.Info.Ptxt_per_elems != 1) {
    fmt::println("Setup Bad");
  } else {
    fmt::println("Setup Good");
  }
  for (u64 i = 0; i < N; ++i) {
    if (DB.getElem(i) != (i + 1)) {
      fmt::println("Get Bad: {} != {}", DB.getElem(i), i + 1);
    }
  }
}

void test_DB_Large_elements() {
  fmt::println("\n>>>> Large emlements");
  u64 N = 4, d = 12;
  std::vector<u64> vals{1, 2, 3, 4};
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);
  Database DB{N, d, &p, vals};

  if (DB.Info.Elems_per_ptxt != 0 || DB.Info.Ptxt_per_elems <= 1) {
    fmt::println("Setup Bad");
  } else {
    fmt::println("Setup Good");
  }
  for (u64 i = 0; i < N; ++i) {
    if (DB.getElem(i) != (i + 1)) {
      fmt::println("Get Bad");
    }
  }
}

int main() {
  test_DB_Medium_elements();
  test_DB_Small_elements();
  test_DB_Large_elements();
}