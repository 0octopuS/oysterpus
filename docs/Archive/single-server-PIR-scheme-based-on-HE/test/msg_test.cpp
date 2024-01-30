#include "fmt/core.h"
#include "matrix.hpp"
#include "pir.hpp"
#include <type_traits>

void test_state() {
  u64 mod = 10087;
  State st{};
  Matrix A{6, 6}, B, C, D;
  A.Rand(mod);
  B.Rand(mod);
  C.Rand(mod);
  D.Rand(mod);

  st.makeMsg(A);
  st.makeMsg(B, C, D);
  st.printMsg();
}

void test_msg() {}

int main() {
  test_state();
  test_msg();
}