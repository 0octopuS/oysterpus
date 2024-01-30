#include "database.hpp"
#include "pir.hpp"

class simplePirClient {
private:
  PirParams Params;
  State A_msg;
  State Hint_msg;
  Matrix SecretKey;
  Matrix Err;
  DBinfo Info;
  bool has_init = false;

public:
  simplePirClient();
  simplePirClient(PirParams, MsgSlice, DBinfo);
  void init(PirParams, MsgSlice, DBinfo);
  Msg query(u64 index);
  u64 extract(u64 index, u64 batch_index, Msg query, Msg reply);
  //   query
};