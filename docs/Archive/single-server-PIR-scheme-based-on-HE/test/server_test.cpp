#include "database.hpp"
#include "pir.hpp"
#include "simplePirServer.hpp"
#include "utils.hpp"
#include <fmt/core.h>
#include <ranges>
#include <vector>
#define LOGQ u64(32)
#define SEC_PARAM u64(1 << 10)
void test_Server_Bandwidth() {
  fmt::println("\n>>>> Bandwidth test");
  u64 N = 1 << 20, d = 2048;
  std::vector<u64> vals{1, 2, 3, 4};
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);
  Database DB{N, d, &p, vals};
  simplePirServer server{p, N, d};

  fmt::println("Executing with entries consisting of {} (>= 1) bits; p is {}; "
               "packing factor is {}; number of DB elems per entry is {}.\n",
               d, p.P, DB.Info.Elems_per_ptxt, DB.Info.Ptxt_per_elems);
  server.getBW();
}

auto main() -> int { test_Server_Bandwidth(); }