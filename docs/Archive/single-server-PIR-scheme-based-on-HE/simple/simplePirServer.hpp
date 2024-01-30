#pragma once

#include "database.hpp"
#include "pir.hpp"
#include <memory>

class simplePirServer {
private:
  Database *DB;
  PirParams Params;
  State St;

public:
  // PirParams pickParams(u64 N, u64 d, u64 n, u64 logq);
  void getBW();

  [[nodiscard]] MsgSlice preprocess();            // return Hint
  [[nodiscard]] MsgSlice preprocess_compressed(); // return Hint
  [[nodiscard]] Msg reply(MsgSlice);              // return query answer
  [[nodiscard]] Msg reply(Msg);
  void reset();

  simplePirServer(u64 _N, u64 _d, u64 _n, u64 _logq, u64 _ele_num,
                  u64 _ele_size);
  simplePirServer(PirParams p, u64 _ele_num, u64 _ele_size);
  simplePirServer(PirParams p, u64 _ele_num, u64 _ele_size, Database *db);
};