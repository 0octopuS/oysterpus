#include "pir.hpp"
#include <vector>

const u64 LOGQ{32};
const u64 SEC_PARAM{1 << 10};

void pir_basic_test() {
  PirParams params;
  u64 N{1 << 10};
  u64 d{8};
  // std::vector<u64> samples = {N, d, SEC_PARAM, LOGQ};
  params.PickParams(N, d, SEC_PARAM, LOGQ);
  params.PrintParams();
}

int main() { pir_basic_test(); }