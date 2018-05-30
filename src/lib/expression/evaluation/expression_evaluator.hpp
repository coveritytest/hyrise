#pragma once

#include <memory>
#include <vector>

#include "boost/variant.hpp"

#include "types.hpp"
#include "all_type_variant.hpp"
#include "null_value.hpp"
#include "expression/logical_expression.hpp"
#include "expression_result.hpp"

namespace opossum {

class AbstractExpression;
class ArithmeticExpression;
class BaseColumn;
class BinaryPredicateExpression;
class CaseExpression;
class Chunk;
class ExistsExpression;
class ExtractExpression;
class FunctionExpression;
class InExpression;
class PQPSelectExpression;

struct BaseColumnMaterialization {
  virtual ~BaseColumnMaterialization() = default;
};

template<typename T>
struct ColumnMaterialization : public BaseColumnMaterialization {
  std::optional<std::vector<bool>> nulls;
  std::vector<T> values;
};

class ExpressionEvaluator final {
 public:
  // For Expressions that do not reference any columns (e.g. in the LIMIT clause)
  ExpressionEvaluator() = default;

  // For Expressions that reference Columns from a single table
  explicit ExpressionEvaluator(const std::shared_ptr<const Chunk>& chunk);

  std::shared_ptr<BaseColumn> evaluate_expression_to_column(const AbstractExpression& expression);

  template<typename T>
  ExpressionResult<T> evaluate_expression(const AbstractExpression& expression);
//
//  template<typename T>
//  ExpressionResult<T> evaluate_function_expression(const FunctionExpression& expression);

  template<typename T>
  ExpressionResult<T> evaluate_arithmetic_expression(const ArithmeticExpression& expression);

  template<typename T>
  ExpressionResult<T> evaluate_logical_expression(const LogicalExpression& expression);

  template<typename T>
  ExpressionResult<T> evaluate_binary_predicate_expression(const BinaryPredicateExpression& expression);

  template<typename T>
  ExpressionResult<T> evaluate_in_expression(const InExpression& in_expression);

  template<typename T>
  ExpressionResult<T> evaluate_select_expression_for_chunk(const PQPSelectExpression &expression);

  template<typename T>
  ExpressionResult<T> evaluate_select_expression_for_row(const PQPSelectExpression& expression, const ChunkOffset chunk_offset);

  template<typename T>
  ExpressionResult<T> evaluate_case_expression(const CaseExpression& case_expression);
//
//  template<typename T>
//  ExpressionResult<T> evaluate_extract_expression(const ExtractExpression& extract_expression);
//
//  template<size_t offset, size_t count>
//  ExpressionResult<std::string> evaluate_extract_substr(const ExpressionResult<std::string>& from_result);
//
//  template<typename T>
//  ExpressionResult<T> evaluate_exists_expression(const ExistsExpression& exists_expression);

  template<typename R, typename Functor>
  ExpressionResult<R> evaluate_binary_with_default_null_logic(const AbstractExpression& left_expression,
                                      const AbstractExpression& right_expression);

  template<typename R, typename Functor>
  ExpressionResult<R> evaluate_binary_with_custom_null_logic(const AbstractExpression& left_expression,
                                                 const AbstractExpression& right_expression);

  template<typename Functor>
  void resolve_to_expression_result_views(const AbstractExpression &left_expression,
                                          const AbstractExpression &right_expression, const Functor &fn);

  template<typename Functor>
  void resolve_to_expression_results(const AbstractExpression &left_expression,
                                          const AbstractExpression &right_expression, const Functor &fn);

  template<typename Functor>
  void resolve_to_expression_result(const AbstractExpression &expression, const Functor &fn);

 private:
  template<typename A, typename B>
  static ChunkOffset _result_size(const A& a, const B& b);

  static std::vector<bool> _evaluate_default_null_logic(const std::vector<bool>& left, const std::vector<bool>& right);

  std::shared_ptr<const Chunk> _chunk;
  size_t _output_row_count{1};

  std::vector<std::unique_ptr<BaseColumnMaterialization>> _column_materializations;
};

}  // namespace opossum
