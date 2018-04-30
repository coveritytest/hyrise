#pragma once

#include "abstract_dp_algorithm.hpp"
#include "dp_subplan_cache_top_k.hpp"
#include "statistics/basic_cardinality_estimator.hpp"

namespace opossum {

class TableStatisticsCache;

class DpCcpTopK : public AbstractDpAlgorithm {
 public:
  explicit DpCcpTopK(const size_t max_entry_count_per_set,
                     const std::shared_ptr<const AbstractCostModel>& cost_model,
                     const std::shared_ptr<AbstractCardinalityEstimator>& cardinality_estimator = std::make_shared<BasicCardinalityEstimator>());

  std::shared_ptr<DpSubplanCacheTopK> subplan_cache();
  std::shared_ptr<const DpSubplanCacheTopK> subplan_cache() const;

 protected:
  void _on_execute() override;
};

}  // namespace opossum
