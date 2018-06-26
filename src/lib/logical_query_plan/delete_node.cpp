#include "delete_node.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>

#include "utils/assert.hpp"

namespace opossum {

DeleteNode::DeleteNode(const std::string& table_name) : AbstractLQPNode(LQPNodeType::Delete), _table_name(table_name) {}

std::string DeleteNode::description(const DescriptionMode description_mode) const {
  std::ostringstream desc;

  desc << "[Delete] Table: '" << _table_name << "'";

  return desc.str();
}

const std::string& DeleteNode::table_name() const { return _table_name; }

std::shared_ptr<AbstractLQPNode> DeleteNode::_shallow_copy_impl(LQPNodeMapping & node_mapping) const {
  return DeleteNode::make(_table_name);
}

bool DeleteNode::_shallow_equals_impl(const AbstractLQPNode& rhs, const LQPNodeMapping & node_mapping) const {
  const auto delete_node_rhs = static_cast<const DeleteNode&>(rhs);
  return _table_name == delete_node_rhs._table_name;
}

}  // namespace opossum
