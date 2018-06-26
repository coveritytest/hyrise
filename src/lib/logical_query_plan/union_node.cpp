#include "union_node.hpp"

#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "constant_mappings.hpp"
#include "expression/expression_utils.hpp"
#include "utils/assert.hpp"

namespace opossum {

UnionNode::UnionNode(const UnionMode union_mode) : AbstractLQPNode(LQPNodeType::Union), union_mode(union_mode) {}

std::string UnionNode::description(const DescriptionMode description_mode) const { return "[UnionNode] Mode: " + union_mode_to_string.at(union_mode); }

const std::vector<std::shared_ptr<AbstractExpression>>& UnionNode::output_column_expressions() const {
  Assert(expressions_equal(left_input()->output_column_expressions(), right_input()->output_column_expressions()), "Input Expressions must match");
  return left_input()->output_column_expressions();
}

std::shared_ptr<TableStatistics> UnionNode::derive_statistics_from(
const std::shared_ptr<AbstractLQPNode>& left_input, const std::shared_ptr<AbstractLQPNode>& right_input) const {
  Fail("Statistics for UNION not yet implemented");
}

std::shared_ptr<AbstractLQPNode> UnionNode::_shallow_copy_impl(LQPNodeMapping & node_mapping) const {
  return UnionNode::make(union_mode);
}

bool UnionNode::_shallow_equals_impl(const AbstractLQPNode& rhs, const LQPNodeMapping & node_mapping) const {
  const auto& union_node = static_cast<const UnionNode&>(rhs);
  return union_mode == union_node.union_mode;
}

}  // namespace opossum
