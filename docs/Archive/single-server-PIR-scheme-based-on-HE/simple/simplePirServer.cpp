#include "simplePirServer.hpp"
#include "database.hpp"
#include "fmt/core.h"

// PirParams simplePirServer::pickParams(u64 N, u64 d, u64 n, u64 logq) {
//   PirParams final_Params{};
//   bool found = false;

//   for (u64 mod_p = 2;; ++mod_p) {
//     auto [l, m] = ApproxSquareDatabaseDims(N, d, mod_p);
//     PirParams temp_Params{.N = n, .L = l, .M = m, .Logq = logq};
//   }
// }

simplePirServer::simplePirServer(u64 _N, u64 _d, u64 _n, u64 _logq,
                                 u64 _ele_num, u64 _ele_size) {
  // >>> Init params
  Params.PickParams(_N, _d, _n, _logq);
  DB = new Database(_ele_num, _ele_size, &Params);

  // >>> Init State
  Matrix A{Params.M, Params.N}; // new state
  A.Rand(
      1ull << Params.Logq); // generate Random Matrix A as the public parameter
  // fmt::print("A:\n");
  // A.print();
  St.makeMsg(A); // record the State

  // >>>
}
simplePirServer::simplePirServer(PirParams p, u64 _ele_num, u64 _ele_size) {
  Params = p;
  DB = new Database(_ele_num, _ele_size, &Params);

  // >>> Init State
  Matrix A{Params.M, Params.N};                               // new state
  std::random_device r;                                       // 1
  std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()}; // 2
  A.Rand_By_Seed(
      seed,
      1ull << Params.Logq); // generate Random Matrix A as the public parameter
  // fmt::print("A dim: {}-{}\n", A.Rows, A.Cols);
  // fmt::print("A:\n");
  // A.print();
  St.makeMsg(A); // record the State

  // >>>
}
simplePirServer::simplePirServer(PirParams p, u64 _ele_num, u64 _ele_size,
                                 Database *db) {
  Params = p;
  DB = db;

  // >>> Init State
  Matrix A{Params.M, Params.N}; // new state
  A.Rand(
      1ull << Params.Logq); // generate Random Matrix A as the public parameter
  // fmt::print("A:\n");
  // A.print();
  St.makeMsg(A); // record the State

  // >>>
}
void simplePirServer::getBW() {
  double offline_download =
      (double)(Params.L * Params.N * Params.Logq) / (8.0 * 1024.0);
  double online_upload = (double)(Params.M * Params.Logq) / (8.0 * 1024.0);
  double online_dowload = (double)(Params.L * Params.Logq) / (8.0 * 1024.0);
  fmt::println("offline_download = {} KB", offline_download);
  fmt::println("online_upload    = {} KB", online_upload);
  fmt::println("online_dowload   = {} KB", online_dowload);
}

MsgSlice simplePirServer::preprocess() {
  auto A = St.Data[0];
  // auto H = DB->Data->matrixMul(&A);
  // A.print();
  Msg H_msg{}, A_msg{};
  // H_msg.makeMsg(DB->Data->matrixMul(&A));
  H_msg.makeMsg(Matrix{DB->Data->Rows, A.Cols});
  A_msg.makeMsg(A);

  DB->Data->add(Params.P / 2);
  DB->squish();
  MsgSlice offline{};
  offline.makeMsgSlice(A_msg, H_msg);

  // auto offline_download =
  //     (Params.L * Params.N * u64(Params.Logq)) / (8.0 * 1024.0);
  // fmt::println("offline_download size:{}", offline_download);
  return offline;
}

MsgSlice simplePirServer::preprocess_compressed() {
  auto A = St.Data[0];
  // auto H = DB->Data->matrixMul(&A);
  // A.print();
  Msg H_msg{}, A_msg{};
  H_msg.makeMsg(DB->Data->matrixMul(&A));
  A_msg.makeMsg(A);
  DB->Data->add(Params.P / 2);
  DB->squish();
  MsgSlice offline{};
  offline.makeMsgSlice(A_msg, H_msg);

  // auto offline_download =
  //     (Params.L * Params.N * u64(Params.Logq)) / (8.0 * 1024.0);
  // fmt::println("offline_download size:{}", offline_download);
  return offline;
}
Msg simplePirServer::reply(MsgSlice qs) {
  Matrix ans{};
  auto query_num = qs.size();
  auto batch_size = DB->Data->Rows / query_num;
  u64 last = 0;

  for (u64 batch = 0; batch < query_num; ++batch) {
    if (batch == query_num - 1) {
      batch_size = DB->Data->Rows - last;
    }
    auto DBrows = DB->Data->selectRows(last, batch_size);
    fmt::print("Matrix1 {}x{}\n", DBrows.Rows, DBrows.Cols);

    // fmt::println("{}", qs.Data[batch].Data[0]);
    auto temp = DBrows.matrixMulVecPacked(&qs.Data[batch].Data[0],
                                          DB->Info.Basis, DB->Info.Squishing);
    // auto temp = DBrows.matrixMul(&qs.Data[batch].Data[0]);
    // temp.print();
    fmt::print("ans: {}x{}\n", temp.Rows, temp.Cols);

    ans.concat(&temp);
    last += batch_size;
  }
  Msg Reply;
  Reply.makeMsg(ans);

  return Reply;
}

void simplePirServer::reset() {
  // Uncompress the database, and map its entries to the range [-p/2, p/2].
  DB->unsquish();
  DB->Data->sub(Params.P / 2);
}