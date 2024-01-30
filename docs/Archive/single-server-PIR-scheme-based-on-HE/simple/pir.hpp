#pragma once
#include "Matrix.hpp"
#include "utils.hpp"
#include <cassert>
#include <cmath>
#include <fmt/core.h>
#include <ranges>
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>

// Database size N: 226 228 230 234 238 242
// Plaintext modulus p: 991 833 701 495 350 247

struct PirParams {
  u64 N;        // LWE secret dimension
  double sigma; // lWE error distribution stddev
  u64 L;        // DB height
  u64 M;        // DB width
  u64 Logq;     // (logarithm of) ciphertext modulus
  u64 P;        // plaintext modulus

  u64 Delta();
  u64 delta();
  u64 Round(u64 x);
  // void PickParams()
  void PickParams(u64 samples); // for simple pir
  void PickParams(u64 _N, u64 _d, u64 _n, u64 _logq);

  void PrintParams();
};

struct PirParamsPreset {
  u64 logn;
  u64 logm;
  u64 logq;
  double sigma;
  u64 log_p_simple;
  u64 mod1;
  u64 mod2;
};

template <typename M>
concept MatrixPack = std::is_same<Matrix, M>::value;

// struct State {
//   std::vector<Matrix> Data;
//   template <MatrixPack M> State makeState(M mats) {
//     Data.emplace_back(mats);
//     return *this;
//   }
//   template <MatrixPack F, MatrixPack... M> State makeState(F first, M...
//   mats) {
//     Data.emplace_back(first);
//     makeState(mats...);
//     return *this;
//   }
//   State makeState(std::vector<Matrix> mats);
//   u64 size() { return Data.size(); }
//   void printState();
// };

struct Msg {
  std::vector<Matrix> Data;
  template <MatrixPack M> Msg makeMsg(M mats) {
    Data.emplace_back(mats);
    return *this;
  }
  template <MatrixPack F, MatrixPack... M> Msg makeMsg(F first, M... mats) {
    Data.emplace_back(first);
    makeMsg(mats...);
    return *this;
  }
  u64 size() { return Data.size(); }
  Msg makeMsg(std::vector<Matrix> mats);
  void printMsg();
};

typedef Msg State;

template <typename M>
concept MsgPack = std::is_same<Msg, M>::value;
struct MsgSlice {
  std::vector<Msg> Data;
  u64 size() { return Data.size(); }
  template <MsgPack M> MsgSlice makeMsgSlice(M mats) {
    Data.emplace_back(mats);
    return *this;
  }
  template <MsgPack F, MsgPack... M> MsgSlice makeMsgSlice(F first, M... mats) {
    Data.emplace_back(first);
    makeMsgSlice(mats...);
    return *this;
  }
  void print();
};

struct Msg_Compressed {
  std::seed_seq Data;
};