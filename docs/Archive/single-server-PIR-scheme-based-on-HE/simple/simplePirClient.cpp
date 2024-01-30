#include "simplePirClient.hpp"
#include "fmt/core.h"
#include <vector>

simplePirClient::simplePirClient() { has_init = false; }

simplePirClient::simplePirClient(PirParams p, MsgSlice offline, DBinfo db)
    : Params(p), A_msg(offline.Data[0]), Hint_msg(offline.Data[1]),
      SecretKey(p.N, 1), Err(p.M, 1), Info(db) {
  SecretKey.Rand(1ull << Params.Logq);
  Err.Gauss(); // generate err matrix, follow gaussian
  has_init = true;
}
void simplePirClient::init(PirParams p, MsgSlice offline, DBinfo db) {
  Params = p;
  A_msg = offline.Data[0];
  Hint_msg = offline.Data[1];
  SecretKey.init(p.N, 1);
  SecretKey.Rand(1ull << Params.Logq);
  Err.init(p.M, 1);
  Err.Gauss(); // generate err matrix, follow gaussian
  Info = db;
  has_init = true;
}
Msg simplePirClient::query(u64 index) {

  if (has_init == false) {
    fmt::println("Client need to be inited!");
    return {};
  }
  auto A = A_msg.Data[0];
  // auto A = Hint.Data[0];
  // fmt::println(" >> A");
  // A.print();
  // fmt::println(" >> secret");
  // SecretKey.print();

  auto query = A.matrixMul(&SecretKey);
  query.matrixAdd(&Err);
  query.Data.at(index % Params.M).at(0) += Params.Delta();
  fmt::println("Delta:{}", Params.Delta());

  if (Params.M % Info.Squishing != 0) {
    query.appendZeros(Info.Squishing - (Params.M % Info.Squishing));
  }
  // query.print();
  Msg Query{};
  Query.makeMsg(query);
  return Query;
}

// Msg simplePirClient::extract() {}

u64 simplePirClient::extract(u64 index, u64 batch_index, Msg query, Msg reply) {
  if (has_init == false) {
    fmt::println("Client need to be inited!");
    return {};
  }
  auto H = Hint_msg.Data[0];
  auto reply_matrix = reply.Data[0];
  auto ratio = Params.P / 2;
  u64 offset = 0;
  for (u64 j = 0; j < Params.M; ++j) {
    offset += ratio * query.Data[0].get(j, 0);
  }
  offset %= (1ull << Params.Logq);
  offset = (1ull << Params.Logq) - offset;
  // fmt::println("offset: {}", offset);

  auto row = index / Params.M;
  auto interm = H.matrixMul(&SecretKey);
  reply_matrix.matrixSub(&interm);

  std::vector<u64> vals;
  for (u64 j = row * Info.Ptxt_per_elems; j < (row + 1) * Info.Ptxt_per_elems;
       ++j) {
    u64 noised = reply_matrix.get(j, 0) + offset;
    u64 denoised = Params.Round(noised);
    vals.emplace_back(denoised);
  }
  reply_matrix.matrixAdd(&interm);

  return reconstructElem(vals, index, Info);
}
