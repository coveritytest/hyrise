#pragma once

#include <vector>

#include "boost/variant.hpp"
#include "boost/variant/apply_visitor.hpp"

#include "storage/column_iterables/column_iterator_values.hpp"
#include "storage/create_iterable_from_column.hpp"
#include "null_value.hpp"
#include "utils/assert.hpp"

namespace opossum {

template<typename T>
class ExpressionResultSeries {
 public:
  using Type = T;

  ExpressionResultSeries(const std::vector<T>& values, const std::vector<bool>& nulls):
  _values(values), _nulls(nulls) {}

  bool is_series() const { return true; }
  bool is_literal() const { return false; }
  bool is_nullable() const { return !_nulls.empty(); }

  const T& value(const size_t idx) const {
    DebugAssert(idx < _values.size(), "Index out of range");
    return _values[idx];
  }

  size_t size() const { return _values.size(); }

  bool null(const size_t idx) const {
    DebugAssert(idx < _values.size(), "Index out of range");
    return _nulls[idx];
  }

 private:
  const std::vector<T>& _values;
  const std::vector<bool>& _nulls;
};

template<typename T>
class ExpressionResultNonNullSeries {
 public:
  using Type = T;

  explicit ExpressionResultNonNullSeries(const std::vector<T>& values):
    _values(values){}

  bool is_series() const { return true; }
  bool is_literal() const { return false; }
  bool is_nullable() const { return false; }

  size_t size() const { return _values.size(); }

  const T& value(const size_t idx) const {
    DebugAssert(idx < _values.size(), "Index out of range");
    return _values[idx];
  }

  bool null(const size_t idx) const { return false;  }

 private:
  const std::vector<T>& _values;
};

template<typename T>
class ExpressionResultLiteral {
 public:
  using Type = T;

  ExpressionResultLiteral(const T& value, const bool null):
  _value(value), _null(null) {}

  bool is_series() const { return false; }
  bool is_literal() const { return true; }
  bool is_nullable() const { return _null; }

  size_t size() const { return 1u; }

  const T& value(const size_t = 0) const { return _value; }
  bool null(const size_t = 0) const { return _null; }

 private:
  T _value;
  bool _null;
};

template<typename T>
class ExpressionResult {
 public:
  using Type = T;

  ExpressionResult() = default;

  ExpressionResult(std::vector<T> values, std::vector<bool> nulls = {false}):
    values(std::move(values)), nulls(std::move(nulls)) {
    Assert(!this->values.empty() && !this->nulls.empty(), "Can't handle empty ExpressionResult");
  }

  bool is_series() const { return size() > 1; }
  bool is_literal() const { return size() == 1; }
  bool is_nullable() const { return nulls.size() > 1 || nulls.front(); }

  ExpressionResultLiteral<T> to_literal() const {
    Assert(is_literal(), "Can't turn non-literal to literal");
    const auto null = is_nullable() ? nulls.front() : false;
    return {values.front(), null};
  }

  ExpressionResultSeries<T> to_series() const {
    Assert(is_nullable(), "Can't turn non-nullable to nullable series");
    return {values, nulls};
  }

  ExpressionResultNonNullSeries<T> to_non_null_series() const {
    Assert(!is_nullable(), "Can't turn nullable to nonnullable series");
    return ExpressionResultNonNullSeries<T>{values};
  }

  const T& value(const size_t idx) const {
    DebugAssert(values.size() == 1 || idx < values.size(), "Invalid ExpressionResult state");
    return values[std::min(idx, values.size() - 1)];
  }

  bool null(const size_t idx) const {
    DebugAssert(nulls.size() == 1 || idx < nulls.size(), "Invalid ExpressionResult state");
    return nulls[std::min(idx, nulls.size() - 1)];
  }

  size_t size() const { return values.size(); }

  std::vector<T> values;
  std::vector<bool> nulls;
};

/**
 * Resolve ExpressionResult<T> to ExpressionResultSeries<T>, ExpressionResultNonNullSeries<T> or
 * ExpressionResultLiteral<T>
 */
template<typename T, typename Functor>
void resolve_expression_result_to_view(const ExpressionResult<T> &result, const Functor &fn) {
  if (result.is_series()) {
    if (result.is_nullable()) {
      fn(result.to_series());
    } else {
      fn(result.to_non_null_series());
    }
  } else {
    fn(result.to_literal());
  }
};

}  // namespace opossum