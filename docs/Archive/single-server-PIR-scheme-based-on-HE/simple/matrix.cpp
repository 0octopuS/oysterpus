#include "matrix.hpp"
#include "fmt/core.h"
#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <vector>

// Matrix::Matrix(const Matrix &other){
//   Rows = other.Rows;
//   Cols = other.Cols;
//   Data = other.Data;
// }

// Matrix &Matrix::operator=(const Matrix &other) {}

u64 Matrix::size() { return Rows * Cols; }
u64 Matrix::get(u64 x, u64 y) { return Data.at(x).at(y); }
void Matrix::set(u64 x, u64 y, u64 val) {
  // fmt::println("x = {}, y = {}", x, y);
  if (x <= Rows && y <= Cols)
    Data[x][y] = val;
  else {
    throw std::invalid_argument("Too many rows");
  }
}

Matrix *Matrix::Zero() {
  for (auto &rol : Data) {
    for (auto &i : rol) {
      i = 0;
    }
  }
  return this;
}
Matrix *Matrix::Fix(u64 mod) {
  u32 j = 0;
  for (auto &rol : Data) {
    for (auto &i : rol) {
      i = (j++) % mod;
    }
  }
  return this;
}

Matrix *Matrix::Gauss() {
  for (auto &rol : Data) {
    for (auto &i : rol) {
      i = gaussSample();
    }
  }
  return this;
}

Matrix *Matrix::Rand(u64 mod) {
  std::random_device r;
  // Choose a random mean between 1 and 6
  std::default_random_engine e1(r());
  std::uniform_int_distribution<i64> rand_ele(0, mod);
  for (auto &rol : Data) {
    for (auto &i : rol) {
      i = rand_ele(e1);
    }
  }
  return this;
}

Matrix *Matrix::Rand_By_Seed(std::seed_seq &seed, u64 mod) {
  // std::random_device r{};
  // Choose a random mean between 1 and 6
  std::default_random_engine e1(seed);
  std::uniform_int_distribution<i64> rand_ele(0, mod);
  for (auto &rol : Data) {
    for (auto &i : rol) {
      i = rand_ele(e1);
    }
  }
  return this;
}

// ----------------- change size -----------------

void Matrix::transpose() {
  std::vector<std::vector<u32>> newData(Cols, std::vector<u32>(Rows));
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      newData[j][i] = Data[i][j];
    }
  }
  std::swap(Rows, Cols);
  std::swap(newData, Data);
}
void Matrix::concat(const Matrix *b) {
  if (Cols == 0 && Rows == 0) {
    Rows = b->Rows;
    Cols = b->Cols;
    Data = b->Data;
    return;
  }
  if (Cols != b->Cols) {
    fmt::print("Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols, b->Rows,
               b->Cols);
  }
  Rows += b->Rows;
  Data.insert(Data.end(), b->Data.begin(), b->Data.end());
}

void Matrix::concatCols(u64 n) {
  if (n == 1)
    return;
  if (Cols % n != 0) {
    fmt::print("n does not divide num cols");
    return;
  }
  Matrix b{Rows * n, Cols / n};
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      u64 c = j / n, r = i + Rows * (j % n);
      b.Data[r][c] = Data[i][j];
    }
  }
  std::swap(*this, b);
}

void Matrix::appendZeros(u64 n) {
  Matrix zero(n, 1);
  concat(&zero);
}

void Matrix::dropLastRows(u64 n) {
  Rows -= n;
  Data.erase(Data.end() - 8, Data.end());
}
// ----------------------------------------------
// ----------------- Calculator -----------------
// ----------------------------------------------

void Matrix::add(u64 val) {
  for (auto &rows : Data) {
    for (auto &i : rows) {
      i += val;
    }
  }
}
void Matrix::addAt(u64 x, u64 y, u64 val) {
  if (x < Rows && y < Cols) {
    Data[x][y] += val;
  }
}
void Matrix::matrixAdd(Matrix *b) {
  if (Cols != b->Cols || Rows != b->Rows) {
    fmt::print("Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols, b->Rows,
               b->Cols);
    return;
  }
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      Data[i][j] += b->Data[i][j];
    }
  }
}
void Matrix::operator+(Matrix *b) { matrixAdd(b); }

void Matrix::sub(u64 val) {
  for (auto &rows : Data) {
    for (auto &i : rows) {
      i -= val;
    }
  }
}
void Matrix::subAt(u64 x, u64 y, u64 val) {
  if (x < Rows && y < Cols) {
    Data[x][y] -= val;
  }
}
void Matrix::matrixSub(Matrix *b) {
  if (Cols != b->Cols || Rows != b->Rows) {
    fmt::print("Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols, b->Rows,
               b->Cols);
    return;
  }
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      Data[i][j] -= b->Data[i][j];
    }
  }
}
void Matrix::operator-(Matrix *b) { matrixAdd(b); }
void Matrix::mul(u64 val) {
  for (auto &rows : Data) {
    for (auto &i : rows) {
      i *= val;
    }
  }
}
void Matrix::mulAt(u64 x, u64 y, u64 val) {
  if (x < Rows && y < Cols) {
    Data[x][y] *= val;
  }
}
Matrix Matrix::matrixMul(Matrix *b) {

  if (b->Cols == 1) {
    return matrixMulVec(b);
  }
  if (Cols != b->Rows) {
    fmt::print("matrixMul: Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols,
               b->Rows, b->Cols);
  }
  Matrix out(Rows, b->Cols);
  out.Zero();
  // std::vector<std::vector<u32>> out(Rows, std::vector<u64>(b->Cols));
  u64 bCols = b->Cols;
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 k = 0; k < Cols; ++k) {
      for (u64 j = 0; j < bCols; ++j) {
        out.Data[i][j] += Data[i][k] * b->Data[k][j];
      }
    }
  }
  return out;
}

void Matrix::matrixMul_inplace(Matrix *b) {
  if (Cols != b->Rows) {
    fmt::print("Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols, b->Rows,
               b->Cols);
  }
  std::vector<std::vector<u32>> out(Rows, std::vector<u32>(b->Cols));
  u64 bCols = b->Cols;
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 k = 0; k < Cols; ++k) {
      for (u64 j = 0; j < bCols; ++j) {
        out[i][j] += Data[i][k] * b->Data[k][j];
      }
    }
  }
  std::swap(out, Data);
}
void Matrix::operator*(Matrix *b) { matrixMul(b); }

Matrix Matrix::matrixMulVec(Matrix *b) {
  // do not require exact match because of DB compression
  if (b->Cols != 1) {
    fmt::print("Matrix {}x{} is not a vector\n", b->Rows, b->Cols);
    throw std::invalid_argument("Matrix size\n");
  }
  if (b->Rows < Cols || b->Rows - Cols > 2) {
    fmt::print("Matrix {}x{} don't match Matrix {}x{}\n", Rows, Cols, b->Rows,
               b->Cols);
    throw std::invalid_argument("Matrix size\n");
  }
  Matrix out(Rows, 1);
  out.Zero();
  for (u64 i = 0; i < Rows; ++i) {
    u32 temp = 0;
    for (u64 j = 0; j < Cols; ++j) {
      temp += Data[i][j] * b->Data[j][0];
    }
    out.Data[i][0] = temp;
  }
  return out;
}

Matrix Matrix::matrixMulVecPacked(Matrix *b, u64 basis, u64 compression) {

  if (Cols * compression != b->Rows) {
    fmt::print("matrixMulVecPacked: Matrix {}x{} don't match Matrix {}x{}\n",
               Rows, Cols, b->Rows, b->Cols);
    throw std::invalid_argument("Matrix size\n");
  }
  if (b->Cols != 1) {
    fmt::print("matrixMulVecPacked: Matrix {}x{} don't match Matrix {}x{}\n",
               Rows, Cols, b->Rows, b->Cols);
    throw std::invalid_argument(
        "matrixMulVecPacked： Secound Matrix is not a vector\n");
  }
  if (compression != 3 && basis != 10) {
    fmt::print("Compression is a hard-coded values!");
    throw std::invalid_argument("Compression is a hard-coded values!");
  }
  Matrix ans{Rows + 8, 1};
  ans.Zero();
  u32 db1, db2, db3, db4, db5, db6, db7, db8;
  u32 val1, val2, val3, val4, val5, val6, val7, val8;
  u32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  size_t index = 0;
  size_t index2 = 0;
  u64 MASK = (1 << basis) - 1;
  u64 basis2 = basis * 2;
  for (size_t i = 0; i < Rows; i += 8) {
    tmp1 = 0;
    tmp2 = 0;
    tmp3 = 0;
    tmp4 = 0;
    tmp5 = 0;
    tmp6 = 0;
    tmp7 = 0;
    tmp8 = 0;

    index2 = 0;
    for (size_t j = 0; j < Cols; ++j) {

      u64 row_index = index / Cols;
      u64 col_index = index % Cols;
      if (7 + row_index >= Data.size() || col_index >= Data[0].size() ||
          index2 >= b->Data.size()) {
        break;
      }
      db1 = Data.at(0 + row_index)
                .at(col_index); // Data[0 + row_index][col_index];
      db2 = Data.at(1 + row_index)
                .at(col_index); // Data[1 + row_index][col_index];
      db3 = Data.at(2 + row_index)
                .at(col_index); // Data[2 + row_index][col_index];
      db4 = Data.at(3 + row_index)
                .at(col_index); // Data[3 + row_index][col_index];
      db5 = Data.at(4 + row_index)
                .at(col_index); // Data[4 + row_index][col_index];
      db6 = Data.at(5 + row_index)
                .at(col_index); // Data[5 + row_index][col_index];
      db7 = Data.at(6 + row_index)
                .at(col_index); // Data[6 + row_index][col_index];
      db8 = Data.at(7 + row_index)
                .at(col_index); // Data[7 + row_index][col_index];
      val1 = db1 & MASK;
      val2 = db2 & MASK;
      val3 = db3 & MASK;
      val4 = db4 & MASK;
      val5 = db5 & MASK;
      val6 = db6 & MASK;
      val7 = db7 & MASK;
      val8 = db8 & MASK;

      tmp1 += val1 * b->Data[index2][0];
      tmp2 += val2 * b->Data[index2][0];
      tmp3 += val3 * b->Data[index2][0];
      tmp4 += val4 * b->Data[index2][0];
      tmp5 += val5 * b->Data[index2][0];
      tmp6 += val6 * b->Data[index2][0];
      tmp7 += val7 * b->Data[index2][0];
      tmp8 += val8 * b->Data[index2][0];

      index2 += 1;

      val1 = (db1 >> basis) & MASK;
      val2 = (db2 >> basis) & MASK;
      val3 = (db3 >> basis) & MASK;
      val4 = (db4 >> basis) & MASK;
      val5 = (db5 >> basis) & MASK;
      val6 = (db6 >> basis) & MASK;
      val7 = (db7 >> basis) & MASK;
      val8 = (db8 >> basis) & MASK;

      tmp1 += val1 * b->Data[index2][0];
      tmp2 += val2 * b->Data[index2][0];
      tmp3 += val3 * b->Data[index2][0];
      tmp4 += val4 * b->Data[index2][0];
      tmp5 += val5 * b->Data[index2][0];
      tmp6 += val6 * b->Data[index2][0];
      tmp7 += val7 * b->Data[index2][0];
      tmp8 += val8 * b->Data[index2][0];

      index2 += 1;

      val1 = (db1 >> basis2) & MASK;
      val2 = (db2 >> basis2) & MASK;
      val3 = (db3 >> basis2) & MASK;
      val4 = (db4 >> basis2) & MASK;
      val5 = (db5 >> basis2) & MASK;
      val6 = (db6 >> basis2) & MASK;
      val7 = (db7 >> basis2) & MASK;
      val8 = (db8 >> basis2) & MASK;

      tmp1 += val1 * b->Data[index2][0];
      tmp2 += val2 * b->Data[index2][0];
      tmp3 += val3 * b->Data[index2][0];
      tmp4 += val4 * b->Data[index2][0];
      tmp5 += val5 * b->Data[index2][0];
      tmp6 += val6 * b->Data[index2][0];
      tmp7 += val7 * b->Data[index2][0];
      tmp8 += val8 * b->Data[index2][0];
      index2 += 1;
      index += 1;
    }
    ans.Data[i][0] += tmp1;
    ans.Data[i + 1][0] += tmp2;
    ans.Data[i + 2][0] += tmp3;
    ans.Data[i + 3][0] += tmp4;
    ans.Data[i + 4][0] += tmp5;
    ans.Data[i + 5][0] += tmp6;
    ans.Data[i + 6][0] += tmp7;
    ans.Data[i + 7][0] += tmp8;
    index += Cols * 7;
  }

  ans.dropLastRows(8);
  return ans;
}

// ----------------------------------------------
// ------------------ Selector ------------------
// ----------------------------------------------

Matrix Matrix::selectRows(u64 offset, u64 num) {
  if (offset == 0 && Rows == num) {
    Matrix ans{};
    ans = *this;
    return ans;
  }
  if (offset > Rows) {
    throw std::invalid_argument("Not enough rows");
  }
  if (offset + num < Rows) {
    Matrix ans{num, Cols};
    for (u64 i = offset, j = 0; i < offset + num; ++i, ++j) {
      ans.Data[j] = Data[i];
    }
    return ans;
  }
  Matrix ans{Rows - offset, Cols};
  for (u64 i = offset, j = 0; i < Rows; ++i, ++j) {
    ans.Data[j] = Data[i];
  }
  return ans;
}
Matrix Matrix::selectCols(u64 offset) {
  Matrix ans{};
  if (Cols == 1) {
    ans = *this;
    return ans;
  }
  Matrix newCol{Rows, 1};
  for (u64 j = 0; j < Rows; ++j) {
    newCol.Data[0][j] = Data[j][offset];
  }
  return newCol;
}

// ----------------------------------------------
// ------------------ Compress ------------------
// ----------------------------------------------

// make a database slimmer
void Matrix::squish(u64 basis, u64 delta) {
  // m x n ==> m x (n + d - 1)/d
  Matrix newM(Rows, (Cols + delta - 1) / delta);
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      for (u64 k = 0; k < delta; ++k) {
        // compute the col index, if in-bound
        if (delta * j + k < Cols) {
          auto val = get(i, delta * j + k);
          newM.Data[i][j] += (val << (k * basis));
        }
      }
    }
  }
  std::swap(*this, newM);
}
void Matrix::unsquish(u64 basis, u64 delta, u64 cols) {
  Matrix origM(Rows, cols);
  u64 mask = (1ull << basis) - 1;
  for (u64 i = 0; i < Rows; ++i) {
    for (u64 j = 0; j < Cols; ++j) {
      for (u64 k = 0; k < delta; ++k) {
        origM.Data[i][j * delta + k] = (get(i, j) >> (k * basis)) & mask;
      }
    }
  }
  std::swap(*this, origM);
}

void Matrix::print() {
  fmt::println("========== {}x{} matrix ==========", Rows, Cols);
  if (Rows <= 20 && Cols <= 20) {
    for (auto &r : Data) {
      for (auto &c : r) {
        fmt::print("{:5d} ", c);
      }
      fmt::print("\n");
    }
  } else {
    for (auto i = 0; i < std::min(2ull, Rows); ++i) {
      for (auto j = 0; j < std::min(2ull, Cols); ++j) {
        fmt::print("{:5d} ", Data[i][j]);
      }
      if (Cols > 2) {
        fmt::print("...");
      }
      if (Cols >= 2) {
        fmt::print("{:5d}\n", Data[i][Cols - 1]);
      } else {
        fmt::print("\n");
      }
    }
    if (Rows >= 2)
      fmt::print("{:^5c} {:^5c} {:^5c}\n", ':', ':', ':');
    // if (Rows > 2)
    //   fmt::print("{:5d} {:5d} ... {:5d}\n", Data[Rows - 1][0],
    //              Data[Rows - 1][1], Data[Rows - 1][Cols - 1]);
  }
  fmt::println("===================================");
}