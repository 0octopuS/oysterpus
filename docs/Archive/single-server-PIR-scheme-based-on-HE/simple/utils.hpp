#pragma once
#include <cmath>
#include <random>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t u32;
typedef int32_t i32;
typedef int64_t i64;
typedef uint64_t u64;

void transpose(u32 *out, const u32 *in, size_t rows, size_t cols);

void matMul(u32 *out, const u32 *a, const u32 *b, size_t aRows, size_t aCols,
            size_t bCols);

void matMulTransposedPacked(u32 *out, const u32 *a, const u32 *b, size_t aRows,
                            size_t aCols, size_t bRows, size_t bCols);

void matMulVec(u32 *out, const u32 *a, const u32 *b, size_t aRows,
               size_t aCols);

void matMulVecPacked(u32 *out, const u32 *a, const u32 *b, size_t aRows,
                     size_t aCols);

i64 gaussSample();

// Returns the i-th elem in the representation of m in base p.
// return k_i, m = (k_h, ... , k_0)_p
u64 base_p(u64 p, u64 m, u64 i);

// Returns the element whose base-p decomposition is given by the values in vals
u64 reconstruct_from_base_p(u64 p, std::vector<u64> vals);

// Returns how many entries in Z_p are needed to represent an element in Z_q
u64 compute_expansionFactor(u64 p, u64 logq);
std::pair<u64, u64> approxSquareDims(u64 Ele_num, u64 Ele_size, u64 P);
auto ptxts_per_db(u64 Ele_num, u64 Ele_size, u64 P)
    -> std::tuple<u64, u64, u64>;
// u64 reconstructElem(std::vector<u64> vals, u64 index, DBinfo Info);