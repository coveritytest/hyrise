#include "benchmark/benchmark.h"

#include "storage/storage_manager.hpp"
#include "tpch/tpch_db_generator.hpp"
//#include "operators/jit_operator/jit_aware_lqp_translator.cpp"
#include "sql/sql_pipeline_builder.hpp"
#include "sql/sql_pipeline.hpp"
#include "sql/sql_pipeline_statement.hpp"
#include "table_generator.hpp"

namespace opossum {

class ExpressionPresentationFixture : public benchmark::Fixture {
 public:
  void SetUp(benchmark::State &state) override {
  }

  void TearDown(benchmark::State &state) override {
  }

  const std::string query_simple_where = "SELECT * FROM lineitem WHERE l_extendedprice * 2.0f > l";
  const std::string query_complex_where = "SELECT l_extendedprice*(1.0-l_discount)*(1.0+l_tax) FROM lineitem";
  const std::string query_arithmetics = "SELECT l_extendedprice*(1.0-l_discount)*(1.0+l_tax) FROM lineitem";
  const std::string query_arithmetics = "SELECT l_extendedprice*(1.0-l_discount)*(1.0+l_tax) FROM lineitem";
  const std::string query_arithmetics_nullables = "SELECT a*(100-b)*(100+c) FROM t";
};

BENCHMARK_F(ExpressionPresentationFixture, Arithmetics)(benchmark::State& state) {
  TpchDbGenerator{0.5f}.generate_and_store();

  std::cout << "Num Rows: " << StorageManager::get().get_table("lineitem")->row_count() << std::endl;

  while (state.KeepRunning()) {
    auto pipeline_statement = SQLPipelineBuilder{query_arithmetics}.disable_mvcc().create_pipeline_statement();
    pipeline_statement.get_result_table();
  }

  StorageManager::reset();
}

BENCHMARK_F(ExpressionPresentationFixture, ArithmeticsNullable)(benchmark::State& state) {
  TableGenerator table_generator;
  table_generator.num_columns = 3;
  table_generator.num_rows = 3'000'000;

  StorageManager::get().add_table("t", table_generator.generate_table(200'000, 0.3));

  while (state.KeepRunning()) {
    auto pipeline_statement = SQLPipelineBuilder{query_arithmetics_nullables}.disable_mvcc().create_pipeline_statement();
    pipeline_statement.get_result_table();
  }

  StorageManager::reset();
}



//BENCHMARK_F(ExpressionPresentationFixture, Arithmetics_JIT)(benchmark::State& state) {
//  while (state.KeepRunning()) {
//    auto pipeline_statement = SQLPipelineBuilder{query_arithmetics}.
//      disable_mvcc().
//      with_lqp_translator(std::make_shared<JitAwareLQPTranslator>()).
//      create_pipeline_statement();
//
//    pipeline_statement.get_result_table();
//  }
//}

}  // namespace opossum