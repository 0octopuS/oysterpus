#include "utils.hpp"
#include "database.hpp"
#include <stdio.h>

// Hard-coded, to allow for compiler optimizations:
#define COMPRESSION 3
#define BASIS 10
#define BASIS2 BASIS * 2
#define MASK (1 << BASIS) - 1

void matMul(u32 *out, const u32 *a, const u32 *b, size_t aRows, size_t aCols,
            size_t bCols) {
  for (size_t i = 0; i < aRows; i++) {
    for (size_t k = 0; k < aCols; k++) {
      for (size_t j = 0; j < bCols; j++) {
        out[bCols * i + j] += a[aCols * i + k] * b[bCols * k + j];
      }
    }
  }
}

void matMulTransposedPacked(u32 *out, const u32 *a, const u32 *b, size_t aRows,
                            size_t aCols, size_t bRows, size_t bCols) {
  u32 val, tmp, db;
  u32 tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  u32 db2, db3, db4, db5, db6, db7, db8;
  u32 val2, val3, val4, val5, vl6, val7, val8;
  size_t ind1, ind2;

  if (aRows > aCols) { // when the database rows are long
    ind1 = 0;
    for (size_t i = 0; i < aRows; i += 1) {
      for (size_t k = 0; k < aCols; k += 1) {
        db = a[ind1++];
        val = db & MASK;
        val2 = (db >> BASIS) & MASK;
        val3 = (db >> BASIS2) & MASK;
        for (size_t j = 0; j < bRows; j += 1) {
          out[bRows * i + j] += val * b[k * COMPRESSION + j * bCols];
          out[bRows * i + j] += val2 * b[k * COMPRESSION + j * bCols + 1];
          out[bRows * i + j] += val3 * b[k * COMPRESSION + j * bCols + 2];
        }
      }
    }
  } else { // when the database rows are short
    for (size_t j = 0; j < bRows; j += 8) {
      ind1 = 0;
      for (size_t i = 0; i < aRows; i += 1) {
        tmp = 0;
        tmp2 = 0;
        tmp3 = 0;
        tmp4 = 0;
        tmp5 = 0;
        tmp6 = 0;
        tmp7 = 0;
        tmp8 = 0;
        ind2 = 0;
        for (size_t k = 0; k < aCols; k += 1) {
          db = a[ind1++];
          for (int m = 0; m < COMPRESSION; m++) {
            val = (db >> (m * BASIS)) & MASK;
            tmp += val * b[ind2 + (j + 0) * bCols];
            tmp2 += val * b[ind2 + (j + 1) * bCols];
            tmp3 += val * b[ind2 + (j + 2) * bCols];
            tmp4 += val * b[ind2 + (j + 3) * bCols];
            tmp5 += val * b[ind2 + (j + 4) * bCols];
            tmp6 += val * b[ind2 + (j + 5) * bCols];
            tmp7 += val * b[ind2 + (j + 6) * bCols];
            tmp8 += val * b[ind2 + (j + 7) * bCols];
            ind2++;
          }
        }
        out[bRows * i + j + 0] = tmp;
        out[bRows * i + j + 1] = tmp2;
        out[bRows * i + j + 2] = tmp3;
        out[bRows * i + j + 3] = tmp4;
        out[bRows * i + j + 4] = tmp5;
        out[bRows * i + j + 5] = tmp6;
        out[bRows * i + j + 6] = tmp7;
        out[bRows * i + j + 7] = tmp8;
      }
    }
  }
}

void matMulVec(u32 *out, const u32 *a, const u32 *b, size_t aRows,
               size_t aCols) {
  u32 tmp;
  for (size_t i = 0; i < aRows; i++) {
    tmp = 0;
    for (size_t j = 0; j < aCols; j++) {
      tmp += a[aCols * i + j] * b[j];
    }
    out[i] = tmp;
  }
}

void matMulVecPacked(u32 *out, const u32 *a, const u32 *b, size_t aRows,
                     size_t aCols) {
  u32 db, db2, db3, db4, db5, db6, db7, db8;
  u32 val, val2, val3, val4, val5, val6, val7, val8;
  u32 tmp, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  size_t index = 0;
  size_t index2;

  for (size_t i = 0; i < aRows; i += 8) {
    tmp = 0;
    tmp2 = 0;
    tmp3 = 0;
    tmp4 = 0;
    tmp5 = 0;
    tmp6 = 0;
    tmp7 = 0;
    tmp8 = 0;

    index2 = 0;
    for (size_t j = 0; j < aCols; j++) {
      db = a[index];
      db2 = a[index + 1 * aCols];
      db3 = a[index + 2 * aCols];
      db4 = a[index + 3 * aCols];
      db5 = a[index + 4 * aCols];
      db6 = a[index + 5 * aCols];
      db7 = a[index + 6 * aCols];
      db8 = a[index + 7 * aCols];

      val = db & MASK;
      val2 = db2 & MASK;
      val3 = db3 & MASK;
      val4 = db4 & MASK;
      val5 = db5 & MASK;
      val6 = db6 & MASK;
      val7 = db7 & MASK;
      val8 = db8 & MASK;
      tmp += val * b[index2];
      tmp2 += val2 * b[index2];
      tmp3 += val3 * b[index2];
      tmp4 += val4 * b[index2];
      tmp5 += val5 * b[index2];
      tmp6 += val6 * b[index2];
      tmp7 += val7 * b[index2];
      tmp8 += val8 * b[index2];
      index2 += 1;

      val = (db >> BASIS) & MASK;
      val2 = (db2 >> BASIS) & MASK;
      val3 = (db3 >> BASIS) & MASK;
      val4 = (db4 >> BASIS) & MASK;
      val5 = (db5 >> BASIS) & MASK;
      val6 = (db6 >> BASIS) & MASK;
      val7 = (db7 >> BASIS) & MASK;
      val8 = (db8 >> BASIS) & MASK;
      tmp += val * b[index2];
      tmp2 += val2 * b[index2];
      tmp3 += val3 * b[index2];
      tmp4 += val4 * b[index2];
      tmp5 += val5 * b[index2];
      tmp6 += val6 * b[index2];
      tmp7 += val7 * b[index2];
      tmp8 += val8 * b[index2];
      index2 += 1;

      val = (db >> BASIS2) & MASK;
      val2 = (db2 >> BASIS2) & MASK;
      val3 = (db3 >> BASIS2) & MASK;
      val4 = (db4 >> BASIS2) & MASK;
      val5 = (db5 >> BASIS2) & MASK;
      val6 = (db6 >> BASIS2) & MASK;
      val7 = (db7 >> BASIS2) & MASK;
      val8 = (db8 >> BASIS2) & MASK;
      tmp += val * b[index2];
      tmp2 += val2 * b[index2];
      tmp3 += val3 * b[index2];
      tmp4 += val4 * b[index2];
      tmp5 += val5 * b[index2];
      tmp6 += val6 * b[index2];
      tmp7 += val7 * b[index2];
      tmp8 += val8 * b[index2];
      index2 += 1;
      index += 1;
    }
    out[i] += tmp;
    out[i + 1] += tmp2;
    out[i + 2] += tmp3;
    out[i + 3] += tmp4;
    out[i + 4] += tmp5;
    out[i + 5] += tmp6;
    out[i + 6] += tmp7;
    out[i + 7] += tmp8;
    index += aCols * 7;
  }
}

void transpose(u32 *out, const u32 *in, size_t rows, size_t cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      out[j * rows + i] = in[i * cols + j];
    }
  }
}

std::vector<double> cdf_table{
    0.5,         0.987867,    0.952345,    0.895957,    0.822578,
    0.736994,    0.644389,    0.549831,    0.457833,    0.372034,
    0.295023,    0.22831,     0.172422,    0.127074,    0.0913938,
    0.0641467,   0.0439369,   0.0293685,   0.0191572,   0.0121949,
    0.00757568,  0.00459264,  0.00271706,  0.00156868,  0.000883826,
    0.000485955, 0.000260749, 0.000136536, 6.97696e-05, 3.47923e-05,
    1.69316e-05, 8.041e-06,   3.72665e-06, 1.68549e-06, 7.43923e-07,
    3.20426e-07, 1.34687e-07, 5.52484e-08, 2.21163e-08, 8.63973e-09,
    3.29371e-09, 1.22537e-09, 4.44886e-10, 1.57625e-10, 5.45004e-11,
    1.83896e-11, 6.05535e-12, 1.94583e-12, 6.10194e-13, 1.86736e-13,
    5.57679e-14, 1.62532e-14, 4.62263e-15, 1.28303e-15, 3.47522e-16,
    9.18597e-17, 2.36954e-17, 5.96487e-18, 1.46533e-18, 3.5129e-19,
    8.21851e-20, 1.87637e-20, 4.18062e-21, 9.08991e-22, 1.92875e-22,
    3.99383e-23, 8.07049e-24, 1.5915e-24,  3.06275e-25, 5.75194e-26,
    1.05418e-26, 1.88542e-27, 3.29081e-28, 5.60522e-29, 9.31708e-30,
    1.51135e-30, 2.39247e-31, 3.69594e-32, 5.57187e-33, 8.19735e-34,
    1.17691e-34, 1.64896e-35, 2.25463e-36, 3.00841e-37, 3.91737e-38,
    4.97795e-39, 6.1731e-40,  7.47055e-41, 8.82266e-42, 1.01682e-42,
    1.14363e-43, 1.25523e-44, 1.34449e-45, 1.40537e-46, 1.43357e-47,
    1.42708e-48, 1.38634e-49, 1.31429e-50, 1.21593e-51, 1.0978e-52,
    9.67246e-54, 8.31661e-55, 6.97835e-56, 5.71421e-57, 4.56622e-58,
    3.56086e-59, 2.70987e-60, 2.01252e-61, 1.45858e-62, 1.03161e-63,
    7.12032e-65, 4.79601e-66, 3.15252e-67, 2.02224e-68, 1.26591e-69,
    7.73344e-71, 4.6104e-72,  2.68226e-73, 1.52287e-74, 8.4376e-76,
    4.56219e-77, 2.40727e-78, 1.23958e-79, 6.22901e-81, 3.05465e-82,
    1.46185e-83, 6.82713e-85, 3.11152e-86, 1.3839e-87,
};

i64 gaussSample() {
  i64 x;
  double y;
  std::random_device r;
  // Choose a random mean between 1 and 6
  std::default_random_engine e1(r());
  std::uniform_int_distribution<i64> cdf_index(0, cdf_table.size());
  std::uniform_real_distribution<double> sample(0.0, 1.0);
  while (1) {
    x = cdf_index(e1);
    y = sample(e1);

    if (y < cdf_table[x]) {
      break;
    }
  }
  if (cdf_index(e1) % 2 == 0) {
    x = -x;
  }

  return x;
}

auto base_p(u64 p, u64 m, u64 i) -> u64 {
  for (u64 j = 0; j < i; ++j) {
    m = m / p;
  }
  return (m % p);
}

auto reconstruct_from_base_p(u64 p, std::vector<u64> vals) -> u64 {
  u64 res = 0, coeff = 1;
  for (auto &v : vals) {
    res += coeff * v;
    coeff *= p;
  }
  return res;
}

auto compute_expansionFactor(u64 p, u64 logq) -> u64 {
  auto logp = std::log2(static_cast<double>(p));
  return static_cast<u64>(std::ceil(logq / logp));
}

auto ptxts_per_db(u64 Ele_num, u64 Ele_size, u64 P)
    -> std::tuple<u64, u64, u64> {
  // if elem size <= ptxt size, then a elem can be placed inside
  auto logp = static_cast<u64>(std::log2(static_cast<double>(P)));
  if (Ele_size <= logp) {
    // pack multiple DB elem into a single ptxt
    u64 elem_per_ptxt = logp / Ele_size;
    auto ptxt_per_db =
        static_cast<u64>(std::ceil((double)Ele_num / (double)elem_per_ptxt));
    if (ptxt_per_db == 0 || ptxt_per_db > Ele_num) {
      fmt::println("Ptxt num is {}; N is {}\n", ptxt_per_db, Ele_num);
      throw std::invalid_argument("Bad ptxt num");
    }
    return std::tuple{ptxt_per_db, 1ull, elem_per_ptxt};
  }
  auto ptxt_per_elems = compute_expansionFactor(P, Ele_size);
  return std::tuple{Ele_num * ptxt_per_elems, ptxt_per_elems, 0ull};
}

std::pair<u64, u64> approxSquareDims(u64 Ele_num, u64 Ele_size, u64 P) {
  auto [ptxt_per_db, ptxt_per_elems, _] = ptxts_per_db(Ele_num, Ele_size, P);
  // fmt::println("ptxt_per_db = {}, ptxt_per_elems = {}", ptxt_per_db,
  //              ptxt_per_elems);
  u64 l = std::floor(std::sqrt(static_cast<double>(ptxt_per_db)));
  auto rem = l % ptxt_per_elems;
  if (rem != 0)
    l += ptxt_per_elems - rem;

  u64 m = std::ceil(static_cast<double>(ptxt_per_db) / static_cast<double>(l));
  return std::pair{l, m};
}
