#include "in_expression.hpp"

#include <sstream>

namespace opossum {

InExpression::InExpression(const std::shared_ptr<AbstractExpression2>& value, const std::shared_ptr<AbstractExpression2>& set):
  AbstractPredicateExpression(PredicateCondition::In, {value, set}) {}

const std::shared_ptr<AbstractExpression2>& InExpression::value() const {
  return arguments[0];
}

const std::shared_ptr<AbstractExpression2>& InExpression::set() const {
  return arguments[1];
}

std::shared_ptr<AbstractExpression2> InExpression::deep_copy() const {
  return std::make_shared<InExpression>(value()->deep_copy(), set()->deep_copy());
}

std::string InExpression::as_column_name() const {
  std::stringstream stream;

  Fail("Todo");

  return stream.str();
}

}  // namespace opossum