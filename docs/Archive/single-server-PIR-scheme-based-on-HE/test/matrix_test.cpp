#include "matrix.hpp"
u64 mod = 10087;
auto test_matrix_mul() {
  Matrix a{4, 4}, b{4, 4};
  a.Fix(mod);
  b.Fix(mod);
  auto c = a.matrixMul(&b);
  a.print();
  b.print();
  c.print();
}

auto test_matrix_mul_vector() {
  Matrix a{4, 4}, b{4, 1};
  a.Fix(mod);
  b.Fix(mod);
  auto c = a.matrixMulVec(&b);
  a.print();
  b.print();
  c.print();
}

auto test_matrix_mul_vector_pack() {
  Matrix a{16, 16}, b{18, 1};
  a.Fix(mod);
  a.squish(10, 3);
  a.print();
  b.Fix(mod);
  auto c = a.matrixMulVecPacked(&b, 10, 3);
  b.print();
  c.print();
}

auto test_random_seed() {
  Matrix a{4, 4}, b{4, 4};
  std::random_device r;                                       // 1
  std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()}; // 2
  a.Rand_By_Seed(seed, mod);
  b.Rand_By_Seed(seed, mod);
  a.print();
  b.print();
  Matrix c{1024, 1024}, d{1024, 1024};
  c.Rand_By_Seed(seed, mod);
  d.Rand_By_Seed(seed, mod);
  c.print();
  d.print();
}

auto main() -> int {
  //   test_matrix_mul();
  //   test_matrix_mul_vector();
  // test_matrix_mul_vector_pack();
  test_random_seed();
}