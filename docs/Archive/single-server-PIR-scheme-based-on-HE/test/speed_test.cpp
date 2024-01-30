#include "database.hpp"
#include "pir.hpp"
#include "simplePirClient.hpp"
#include "simplePirServer.hpp"
#include "utils.hpp"
#include <chrono>
#include <fmt/core.h>
#include <iostream>
#include <ranges>
#include <ratio>
#include <vector>

#define LOGQ u64(32)
#define SEC_PARAM u64(1 << 10)

// Benchmark function
void benchmark(void (*function)()) {
  auto start = std::chrono::high_resolution_clock::now();

  // Call the function to be benchmarked
  function();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto duration_ms = duration.count();

  std::cout << "Execution time: " << duration_ms << " milliseconds"
            << std::endl;
}

auto pir_test(std::vector<u64> indices, u64 N, u64 d) {
  fmt::println("Executing pir.");

  // ------------------------------------
  // >> Initial Parameters and database
  // ------------------------------------

  auto start = std::chrono::high_resolution_clock::now();
  PirParams p{};
  p.PickParams(N, d, SEC_PARAM, LOGQ);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto duration_ms = duration.count();

  std::cout << ">>> 1. PickParams Execution time: " << duration_ms
            << " milliseconds" << std::endl;
  Database DB{N, d, &p};
  DB.makeRandomDB(p.P);

  // ------------------------------------
  // >> Initial Server
  // ------------------------------------

  start = std::chrono::high_resolution_clock::now();
  simplePirServer server{p, N, d, &DB};
  server.getBW();
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  duration_ms = duration.count();
  std::cout << ">>> 2. InitialServer Execution time: " << duration_ms
            << " milliseconds" << std::endl;

  start = std::chrono::high_resolution_clock::now();
  auto offline = server.preprocess();
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  duration_ms = duration.count();
  std::cout << ">>> 3. preprocess Execution time: " << duration_ms
            << " milliseconds" << std::endl;

  // ------------------------------------
  // >> Initial Client
  // ------------------------------------
  start = std::chrono::high_resolution_clock::now();
  simplePirClient client{p, offline, DB.Info};
  u64 query_num = indices.size();
  if (DB.Data->Rows / query_num < DB.Info.Ptxt_per_elems) {
    fmt::println("Too many queries to handle!");
    return;
  }
  auto batch_size =
      DB.Data->Rows / (DB.Info.Ptxt_per_elems * query_num) * DB.Data->Cols;
  //   double brandwidth = 0.0;

  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  duration_ms = duration.count();
  std::cout << ">>> 4. InitialClient Execution time: " << duration_ms
            << " milliseconds" << std::endl;

  // ------------------------------------
  // >> start query
  // ------------------------------------

  // std::vector<Msg> client_state;
  fmt::println("Generate query...");

  MsgSlice queries;
  start = std::chrono::high_resolution_clock::now();
  for (u64 i = 0; i < indices.size(); ++i) {
    auto index_to_query = indices[i] + i * batch_size;
    auto q = client.query(index_to_query);
    // q.printMsg();
    // fmt::println("query q.size():{}", q.size());
    queries.Data.emplace_back(q);
    // queries.print();
  }
  //   queries.print();
  //   fmt::println("{}",q)
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  duration_ms = duration.count();
  std::cout << ">>> 5. Genquery Execution time: " << duration_ms
            << " milliseconds" << std::endl;

  // ------------------------------------
  // >> state reply
  // ------------------------------------

  fmt::println("Answering query...");
  start = std::chrono::high_resolution_clock::now();
  auto re = server.reply(queries);
  // fmt::print("re: {}x{}\n", re.Data[0].Rows, re.Data[0].Cols);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  duration_ms = duration.count();
  std::cout << ">>> 6. Reply Execution time: " << duration_ms << " milliseconds"
            << std::endl;

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

int main() {
  // u64 k = 33;
  // for (u64 d = 8; d < 32768; d *= 2) {
  //   u64 N = (1ull << k) / d;
  //   pir_test({262144}, N, d);
  // }
  // pir_test({262144}, 1 << 20, 8);
  //   pir_test({1, 8, 123, 45, 262144}, 1 << 20, 8);
  // pir_test({1, 2, 3, 262144}, 1 << 20, 512);
  // pir_test({1, 2, 3, 262144}, 1 << 20, 1024);
  pir_test({262144}, 1 << 21, 2048);
  // pir_test({1, 2, 3, 262144}, 1 << 20, 2048);
  //  pir_test({262144}, 1 << 21, 4096);
}