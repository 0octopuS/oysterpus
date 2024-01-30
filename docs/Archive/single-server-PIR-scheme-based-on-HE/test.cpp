#include <iostream>
#include <omp.h>
#include <random>
#include <vcruntime.h>
#include <vector>
using namespace std;

// const int N = 100; // Matrix size

// void matrixMultiplication(const float *A, const float *B, float *C, size_t N)
// { #pragma omp parallel num_threads(64)
//   {
//     int numThreads = omp_get_num_threads();
//     printf("num = %d\n", numThreads);

// #pragma omp for
//     for (size_t i = 0; i < N; i++) {
//       for (size_t j = 0; j < N; j++) {
//         C[i * N + j] = 0;
//         for (size_t k = 0; k < N; k++) {
//           C[i * N + j] += A[i * N + k] * B[k * N + j];
//         }
//       }
//     }
//   }
// }
class L {
public:
  int id{};
  std::string s;
  int *pi;
  L() { printf("L()                 %d  [default0 constructor]\n", id); }
  L(int i) : id{i} {
    printf("L()                 %d  [default constructor]\n", id);
  }
  L(const L &l) = delete;
  L(L &&l) = delete;
  L &operator=(const L &) = delete;
  // L &operator=(L &&) = delete;
  // L(const L &l) noexcept : id{l.id + 1} {
  //   printf("L(const L &) %d  [copy constructor]\n", id);
  // }

  // L(L &&l) noexcept : id{l.id + 2} {
  //   printf("L(L &&)      %d  [move constructor]\n", id);
  // }
  // L &operator=(const L &) {
  //   printf("&operator=(const L &) %d[copy assignment]\n", id);
  //   return *this;
  // }
  // L &operator=(L &&) {
  //   printf("&operator=(L &&) %d     [move assignment]\n", id);
  //   return *this;
  // }
  ~L() { printf("~L()                %d  [destructor]\n", id); }
};

int main() {

  auto A = L{};
  L B{20};

  // printf("%d", omp_get_num_procs());
  // printf("%d", omp_get_max_threads());
  // size_t N = 1 << 10;

  // // auto t = std::random_device{};
  // // std::random_device rd;

  // auto A = std::vector<float>(N * N);
  // // std::vector<float> A = std::vector<float>(N * N);

  // auto B = std::vector<float>(N * N);
  // auto C = std::vector<float>(N * N);
  // auto rd = random_device{};
  // auto rng = default_random_engine{rd()};
  // auto dis = uniform_real_distribution<float>(-100.0, 100.0);
  // generate_n(begin(A), N * N, [&]() { return dis(rng); });
  // generate_n(begin(B), N * N, [&]() { return dis(rng); });

  // matrixMultiplication(A.data(), B.data(), C.data(), N);

  // // printf matrix C
  // // ...

  return 0;
}
