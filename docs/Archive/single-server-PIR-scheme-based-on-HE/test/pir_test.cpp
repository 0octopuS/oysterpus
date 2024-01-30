#include "database.hpp"
#include "pir.hpp"
#include "simplePirClient.hpp"
#include "simplePirServer.hpp"
#include "utils.hpp"
#include <fmt/core.h>
#include <ranges>
#include <vector>

#define LOGQ u64(32)
#define SEC_PARAM u64(1 << 10)

auto pir_test(std::vector<u64> indices) {
  fmt::println("Executing pir.");
  u64 N = 1 << 20, d = 8;

  // >> Initial Parameters and database
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);
  Database DB{N, d, &p};
  DB.makeRandomDB(p.P);

  // >> Initial Server
  simplePirServer server{p, N, d, &DB};
  server.getBW();
  auto offline = server.preprocess();

  // >> Initial Client
  simplePirClient client{p, offline, DB.Info};
  u64 query_num = indices.size();
  if (DB.Data->Rows / query_num < DB.Info.Ptxt_per_elems) {
    fmt::println("Too many queries to handle!");
    return;
  }
  auto batch_size =
      DB.Data->Rows / (DB.Info.Ptxt_per_elems * query_num) * DB.Data->Cols;
  double brandwidth = 0.0;

  // >> start query
  // std::vector<Msg> client_state;
  fmt::println("Generate query...");
  MsgSlice queries;
  for (u64 i = 0; i < indices.size(); ++i) {
    auto index_to_query = indices[i] + i * batch_size;
    auto q = client.query(index_to_query);
    // q.printMsg();
    // fmt::println("query q.size():{}", q.size());
    queries.Data.emplace_back(q);

    // queries.print();
  }

  // >> state reply
  fmt::println("Answering query...");
  auto re = server.reply(queries);
  // fmt::print("re: {}x{}\n", re.Data[0].Rows, re.Data[0].Cols);

  server.reset();
  // >> state extract
  for (u64 i = 0; i < indices.size(); ++i) {
    auto index_to_query = indices[i] + i * batch_size;
    fmt::print("index_to_query :{}\n", index_to_query);
    auto val = client.extract(index_to_query, i, queries.Data[i], re);
    if (DB.getElem(index_to_query) != val) {
      fmt::print("Batch {} (querying index {} -- row should be >= {}): Got {} "
                 "instead of {}\n",
                 i, index_to_query, DB.Data->Rows / 4, val,
                 DB.getElem(index_to_query));
      // panic("Reconstruct failed!")
    } else {
      fmt::print("Batch {} (querying index {} -- row should be >= {}): Got {}, "
                 "Except {}\n",
                 i, index_to_query, DB.Data->Rows / 4, val,
                 DB.getElem(index_to_query));
    }
  }

  fmt::println("Success!");
}

auto main() -> int {
  std::vector<u64> query = {0, 0, 0, 0};
  pir_test(query);
}