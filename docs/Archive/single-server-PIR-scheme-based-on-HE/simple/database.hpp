#pragma once

#include "matrix.hpp"
#include "pir.hpp"
#include "utils.hpp"
#include <utility>
#include <vector>

struct DBinfo {
  u64 Ele_num;  // number of DB elements
  u64 Ele_size; // number of bits per DB entry

  // >>> These two are opposite
  u64 Elems_per_ptxt; // number of DB elements per Z_p elem, if log(p) > DB
                      // entry size
  u64 Ptxt_per_elems; // number of Z_p elems per DB elements, if DB entry
                      // size > log(p)

  u64 X;    // tunable param that governs communication,
            // must be in range [1, ptxt_per_elements] and must be a divisor of
            // ptxt_per_elements; represents the number of times the scheme is
            // repeated.
  u64 P;    // plaintext modulus
  u64 Logq; // log of ciphertext modulus

  // for in-memory DB compression
  bool Is_compressed = false;
  u64 Basis = 0;
  u64 Squishing =
      0; // Squishing represents conpressed several consecutive values
  u64 Uncompressed_Cols = 0; // Original col num

  auto compute_ptxt_num();
  DBinfo();
  auto setup(u64 _ele_num, u64 _ele_size, u64 P, u64 logq);
};

// Database contains (DBinfo, Matrix* Data)
// an elem represent an original entry
struct Database {
  DBinfo Info;
  Matrix *Data;

  void squish();
  void unsquish();
  u64 reconstructElem(std::vector<u64> vals, u64 index);
  // get the i-th elem in DB
  u64 getElem(u64 i);

  // Returns how many Z_p elements are needed to represent a database of N
  // entries, each consisting of row_length bits.
  std::tuple<u64, u64, u64> ptxts_per_db();
  std::pair<u64, u64> approxSquareDatabaseDims();
  std::pair<u64, u64> approxDatabaseDims(u64 lower_bound_m);

  void makeRandomDB(u64 mod_p);
  void makeFixDB();
  void makeDB(u64 mod_p, std::vector<u64> vals);
  // Database(u64 _ele_num, u64 _ele_size, PirParams *p);

  // Database(){};
  explicit Database(u64 _ele_num, u64 _ele_size, PirParams *params);
  explicit Database(u64 _ele_num, u64 _ele_size, PirParams *params,
                    std::vector<u64> vals);
};

u64 elements_per_ptxts(u64 _N, u64 _ele_size, u64 _ele_num);

u64 reconstructElem(std::vector<u64> vals, u64 index, DBinfo Info);