#pragma once
#include "utils.hpp"
#include <random>
#include <vector>

struct Matrix {
  u64 Rows{};
  u64 Cols{};
  std::vector<std::vector<u32>> Data{};
  // Constructor
  Matrix() : Rows{0}, Cols(0) {}
  Matrix(u64 x, u64 y) : Rows{x}, Cols(y) {
    Data.resize(x);
    for (auto &row : Data) {
      row.resize(y, 0);
    }
  }
  void init(u64 x = 5, u64 y = 5) {
    Rows = x;
    Cols = y;
    Data.resize(x);
    for (auto &row : Data) {
      row.resize(y, 0);
    }
  }
  // Matrix(const Matrix &other);
  // Matrix &operator=(const Matrix &other);

  // Basic Function
  u64 size();
  u64 get(u64 x, u64 y);
  void set(u64 x, u64 y, u64 val);
  // Fill Matrix

  Matrix *Zero();
  Matrix *Fix(u64 mod);
  Matrix *Gauss();
  Matrix *Rand(u64 mod);
  Matrix *Rand_By_Seed(std::seed_seq &seed, u64 mod);

  // Change size
  void transpose();
  void concat(const Matrix *b);
  void concatCols(u64 n);
  void appendZeros(u64 n);
  void dropLastRows(u64 n);

  // Calculator

  void add(u64 val);
  void addAt(u64 x, u64 y, u64 val);
  void matrixAdd(Matrix *b);
  void operator+(Matrix *b);

  void sub(u64 val);
  void subAt(u64 x, u64 y, u64 val);
  void matrixSub(Matrix *b);
  void operator-(Matrix *b);

  void mul(u64 val);
  void mulAt(u64 x, u64 y, u64 val);
  Matrix matrixMul(Matrix *b);
  void matrixMul_inplace(Matrix *b);
  void operator*(Matrix *b);

  Matrix matrixMulVec(Matrix *b);
  Matrix matrixMulVecPacked(Matrix *b, u64 basis, u64 compression);

  // Selector

  Matrix selectRows(u64 offset, u64 num);
  Matrix selectCols(u64 offset);

  // Compress
  void squish(u64 basis, u64 delta);
  void unsquish(u64 basis, u64 delta, u64 cols);
  void print();
};
