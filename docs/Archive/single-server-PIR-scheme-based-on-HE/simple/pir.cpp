#include "pir.hpp"
#include "fmt/core.h"
#include "utils.hpp"
#include <cstdarg>
#include <stdexcept>
#include <type_traits>
#include <vcruntime_typeinfo.h>

std::vector<PirParamsPreset> preset{
    PirParamsPreset{10, 13, 32, 6.400000, 9, 991, 929},
    PirParamsPreset{10, 14, 32, 6.400000, 9, 833, 781},
    PirParamsPreset{10, 15, 32, 6.400000, 9, 701, 657},
    PirParamsPreset{10, 16, 32, 6.400000, 9, 589, 552},
    PirParamsPreset{10, 17, 32, 6.400000, 8, 495, 464},
    PirParamsPreset{10, 18, 32, 6.400000, 8, 416, 390},
    PirParamsPreset{10, 19, 32, 6.400000, 8, 350, 328},
    PirParamsPreset{10, 20, 32, 6.400000, 8, 294, 276},
    PirParamsPreset{10, 21, 32, 6.400000, 7, 247, 231}};

u64 PirParams::delta() {
  return ceil(static_cast<double>(Logq)) / std::log2(static_cast<double>(P));
}

u64 PirParams::Delta() { return (1ull << Logq) / P; }

u64 PirParams::Round(u64 x) {
  auto _delta = Delta();
  auto v = (x + _delta / 2) / _delta;
  return v % P;
}

void PirParams::PickParams(u64 _N, u64 _d, u64 _n, u64 _logq) {
  bool found = false;

  for (u64 mod_p = 2;; ++mod_p) {
    auto [l, m] = approxSquareDims(_N, _d, mod_p);
    // fmt::println("{} {}", l, m);

    PirParams p{.N = _n, .L = l, .M = m, .Logq = _logq};
    p.PickParams(m);
    // p.PrintParams();
    // fmt::println("P = {}, mod_p = {}, found = {}", p.P, mod_p, found);
    if (p.P < mod_p) {
      if (!found)
        throw std::invalid_argument("Can find params.");
      PrintParams();
      return;
    }

    std::swap(*this, p);
    found = true;
  }
}

void PirParams::PickParams(u64 samples) {
  if (N == 0 || Logq == 0) {
    throw std::invalid_argument("Params: Need to specify n and q");
  }

  u64 num_samples = 0;
  num_samples = samples > num_samples ? samples : num_samples;
  for (auto &_pre : preset) {
    // fmt::println("{} {} {}", 1ull << _pre.logn, 1ull << _pre.logm,
    // _pre.logq);
    if ((N == 1ull << _pre.logn) && (num_samples <= 1ull << _pre.logm) &&
        (Logq == _pre.logq)) {
      sigma = _pre.sigma;
      P = _pre.mod1;
      if (sigma == 0.0 || P == 0) {
        throw std::invalid_argument("Params: invalid parameters.");
      }
      return;
    }
  }
  fmt::print("Searched for {}, {}-by-{},{}.\n", N, L, M, Logq);
  throw std::invalid_argument("Params: No suitable params known!");
}

void PickParams(u64 N, u64 d, u64 n, u64 logq){
    // for (u64 mod_p = 2;; ++mod_p) {
    //   ApproxSquareDatabaseDims
    // }
};
void PirParams::PrintParams() {
  fmt::print("Working with: \n"
             "   n = {}\n"
             "   db size = 2^{:d} (l = {}, m = {})\n"
             "   logq = {}\n"
             "   p = {}\n"
             "   sigma={}\n",
             N, static_cast<u64>(std::log2((double)L) + std::log2((double)M)),
             L, M, Logq, P, sigma);
}

// template <MatrixPack M> State State::makeState(M mats) {
//   Data.insert(mats);
//   return *this;
// }

// template <MatrixPack F, MatrixPack... M>
// State State::makeState(F first, M... mats) {
//   Data.insert(first);
//   return *this;
// }
// State State::makeState(std::vector<Matrix> mats) {
//   Data.insert(Data.begin(), mats.begin(), mats.end());
//   return *this;
// }

void Msg::printMsg() {
  fmt::println("Msg size: {}", Data.size());
  int i = 0;
  for (auto &m : Data) {
    fmt::println("Matrix {}", i++);
    m.print();
  }
}

Msg makeMsg(std::vector<Matrix> mats) {
  Msg msg{};
  msg.Data.insert(msg.Data.begin(), mats.begin(), mats.end());
  return msg;
}
// u64 Msg::size() {}

void MsgSlice::print() {
  fmt::println("MsgSlice size: {}", Data.size());
  int i = 0, j = 0;
  for (auto &m : Data) {
    fmt::println("Msg {}", i++);
    for (auto k : m.Data) {
      fmt::println("Matrix {}", j++);
      k.print();
    }
    j = 0;
  }
}