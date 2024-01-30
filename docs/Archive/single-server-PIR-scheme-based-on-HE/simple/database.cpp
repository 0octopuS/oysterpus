#include "database.hpp"
#include "fmt/core.h"
#include <cmath>
#include <exception>
#include <stdexcept>
#include <vector>
DBinfo::DBinfo() : Ele_num{0}, Ele_size(0), P{0}, Logq{0} {}

auto DBinfo::setup(u64 _ele_num, u64 _ele_size, u64 _P, u64 _Logq) {
  Ele_num = _ele_num;
  Ele_size = _ele_size;
  P = _P;
  Logq = _Logq;
}

Database::Database(u64 _ele_num, u64 _ele_size, PirParams *params) {
  if (_ele_num == 0 || _ele_size == 0) {
    throw std::invalid_argument("Empty databse!");
  }
  Info.setup(_ele_num, _ele_size, params->P, params->Logq);
  auto [ptxt_per_db, ptxt_per_elems, elems_per_ptxt] = ptxts_per_db();
  Info.Ptxt_per_elems = ptxt_per_elems;
  Info.X = ptxt_per_elems;
  Info.Elems_per_ptxt = elems_per_ptxt;

  while (Info.Ptxt_per_elems % Info.X != 0) {
    ++Info.X;
  }
  fmt::println("Total packed DB size is ~{:f} MB\n",
               (double)(params->L * params->M) *
                   std::log2(static_cast<double>(params->P)) /
                   (1024.0 * 1024.0 * 8.0));
  if (ptxt_per_db > params->L * params->M)
    throw std::invalid_argument("Params and database size don't match");
  if (params->L % Info.Ptxt_per_elems != 0) {
    throw std::invalid_argument(
        "Number of DB elems per entry must divide DB height");
  }
  Data = new Matrix(params->L, params->M);
  Data->Zero();
  fmt::println("Matrix size: {}x{}", Data->Rows, Data->Cols);
  fmt::println("Info size: {}x{}", Info.Ele_num, Info.Ele_size);
}

Database::Database(u64 _ele_num, u64 _ele_size, PirParams *params,
                   std::vector<u64> vals)
    : Database(_ele_num, _ele_size, params) {
  makeDB(params->P, vals);
}

void Database::makeDB(u64 mod_p, std::vector<u64> vals) {
  fmt::println("MakeDB");
  if (vals.size() > Info.Ele_num) {
    throw std::invalid_argument("Bad DB size");
  }

  auto col = Data->Cols;
  if (Info.Elems_per_ptxt > 0) {
    u64 index = 0, cur = 0, coeff = 1;
    for (u64 i = 0; i < vals.size(); ++i) {
      cur += (vals[i] * coeff);
      coeff *= (1ull << Info.Ele_size);

      if (((i + 1) % Info.Elems_per_ptxt == 0) || (i == (vals.size() - 1))) {
        // fmt::println("cur = {}, col ={}, val ={}", cur, index / col,
        //              index % col);
        Data->set(index / col, index % col, cur);
        ++index;
        cur = 0;
        coeff = 1;
      }
    }
  } else {
    for (u64 i = 0; i < vals.size(); ++i) {
      for (u64 j = 0; j < Info.Ptxt_per_elems; ++j) {
        // fmt::println("col ={}, val ={}, cur = {}",
        //              (i / col) * Info.Ptxt_per_elems + j, i % col,
        //              base_p(Info.P, vals[i], j));
        Data->set((i / col) * Info.Ptxt_per_elems + j, i % col,
                  base_p(Info.P, vals[i], j));
      }
    }
  }
  Data->sub(mod_p / 2);
}
void Database::makeRandomDB(u64 mod_p) {
  Data->Rand(mod_p);
  Data->sub(mod_p / 2);
}

void Database::squish() {
  Info.Basis = 10;
  Info.Squishing = 3;
  Info.Uncompressed_Cols = Data->Cols;
  Data->squish(Info.Basis, Info.Squishing);
  fmt::println("After squishing, with compression factor {:d} ",
               Info.Squishing);

  // Check that params allow for this compression
  if (Info.P > (1ull << Info.Basis) ||
      Info.Logq < Info.Basis * Info.Squishing) {
    throw std::invalid_argument("Bad params");
  }
}

void Database::unsquish() {
  Data->unsquish(Info.Basis, Info.Squishing, Info.Uncompressed_Cols);
}

u64 Database::reconstructElem(std::vector<u64> vals, u64 index) {
  u64 q = (u64)1 << Info.Logq;
  auto half_P = Info.P / 2;

  // decomposed into Z_p elems, and mapped to [-p/2, p/2]
  for (u64 i = 0; i < vals.size(); ++i) {
    vals[i] = (vals[i] + half_P) % q;
    vals[i] %= Info.P;
  }
  // count the real val
  auto val = reconstruct_from_base_p(Info.P, vals);

  // if 1 ptxt contains several elems, the realval is one elem inside val
  // original base = 1 << ele_size
  // original index = index % elems_per_ptxt
  if (Info.Elems_per_ptxt > 0) {
    val = base_p(((u64)1 << Info.Ele_size), val, index % Info.Elems_per_ptxt);
  }
  return val;
}

u64 Database::getElem(u64 index) {
  if (index >= Info.Ele_num) {
    throw std::invalid_argument("Index out of range");
  }
  u64 newindex = index;
  // if 1 ptxt contains several elems, compute real index
  if (Info.Elems_per_ptxt > 0) {
    newindex = index / Info.Elems_per_ptxt;
  } else {
    newindex = index;
  }

  // comput the 2-dim index [row,col]
  u64 col = newindex % Data->Cols;
  u64 row = newindex / Data->Cols;
  // u64 row = index % Data->Rows;
  // fmt::println("col={},row={},newindex={},DB.Data.Cols={}", col, row,
  // newindex,
  //              Data->Cols);

  std::vector<u64> vals{};
  // for the j
  for (u64 j = row * Info.Ptxt_per_elems; j < (row + 1) * Info.Ptxt_per_elems;
       ++j) {
    vals.emplace_back(Data->get(j, col));
  }

  return reconstructElem(vals, index);
}

auto Database::ptxts_per_db() -> std::tuple<u64, u64, u64> {
  // if elem size <= ptxt size, then a elem can be placed inside
  auto logp = std::log2(static_cast<double>(Info.P));
  if (Info.Ele_size <= logp) {
    // pack multiple DB elem into a single ptxt
    u64 elem_per_ptxt = logp / Info.Ele_size;
    auto ptxt_per_db = static_cast<u64>(
        std::ceil((double)Info.Ele_num / (double)elem_per_ptxt));
    if (ptxt_per_db == 0 || ptxt_per_db > Info.Ele_num) {
      fmt::println("Ptxt num is {}; N is {}\n", ptxt_per_db, Info.Ele_num);
      throw std::invalid_argument("Bad ptxt num");
    }
    Info.Ptxt_per_elems = 1;
    Info.Elems_per_ptxt = elem_per_ptxt;
    return std::tuple{ptxt_per_db, 1ull, elem_per_ptxt};
  }
  auto ptxt_per_elems = compute_expansionFactor(Info.P, Info.Ele_size);
  Info.Ptxt_per_elems = ptxt_per_elems;
  Info.Elems_per_ptxt = 0;
  return std::tuple{Info.Ele_num * ptxt_per_elems, ptxt_per_elems, 0ull};
}

std::pair<u64, u64> Database::approxSquareDatabaseDims() {
  auto [ptxt_per_db, ptxt_per_elems, _] = ptxts_per_db();
  u64 l = std::floor(std::sqrt(static_cast<double>(ptxt_per_db)));
  auto rem = l % ptxt_per_elems;
  if (rem != 0)
    l += ptxt_per_elems - rem;

  u64 m = std::ceil(static_cast<double>(ptxt_per_db) / l);
  return std::pair{l, m};
}

std::pair<u64, u64> Database::approxDatabaseDims(u64 lower_bound_m) {
  auto [l, m] = approxSquareDatabaseDims();
  if (m > lower_bound_m) {
    return std::pair{l, m};
  }

  m = lower_bound_m;
  auto [ptxt_per_db, ptxt_per_elems, _] = ptxts_per_db();
  l = std::ceil(static_cast<double>(ptxt_per_db) / m);

  auto rem = l % ptxt_per_elems;
  if (rem != 0)
    l += ptxt_per_elems - rem;
  return std::pair{l, m};
}

u64 reconstructElem(std::vector<u64> vals, u64 index, DBinfo Info) {
  u64 q = 1ull << Info.Logq;
  auto half_P = Info.P / 2;

  // decomposed into Z_p elems, and mapped to [-p/2, p/2]
  for (u64 i = 0; i < vals.size(); ++i) {
    vals[i] = (vals[i] + half_P) % q;
    vals[i] %= Info.P;
  }
  // count the real val
  auto val = reconstruct_from_base_p(Info.P, vals);

  // if 1 ptxt contains several elems, the realval is one elem inside val
  // original base = 1 << ele_size
  // original index = index % elems_per_ptxt
  if (Info.Elems_per_ptxt > 0) {
    val = base_p((1ull << Info.Ele_size), val, index % Info.Elems_per_ptxt);
  }
  return val;
}
