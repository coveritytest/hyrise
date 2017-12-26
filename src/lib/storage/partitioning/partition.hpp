#pragma once

#include "all_type_variant.hpp"
#include "storage/base_column.hpp"
#include "storage/chunk.hpp"
#include "storage/proxy_chunk.hpp"
#include "types.hpp"
#include "type_cast.hpp"
#include "utils/assert.hpp"

namespace opossum {

class Table;

class Partition {
 public:
  explicit Partition(Table& table);

  void add_column(DataType data_type, bool nullable);
  void append(std::vector<AllTypeVariant> values);
  ChunkID chunk_count() const;
  void create_new_chunk();
  void emplace_chunk(Chunk& chunk);
  Chunk& get_chunk(ChunkID chunk_id);
  const Chunk& get_chunk(ChunkID chunk_id) const;
  ProxyChunk get_chunk_with_access_counting(ChunkID chunk_id);
  const ProxyChunk get_chunk_with_access_counting(ChunkID chunk_id) const;
  TableType get_type() const;
  AllTypeVariant get_value(const ColumnID column_id, const size_t row_number) const;
  uint64_t row_count() const;

 protected:
  std::vector<Chunk> _chunks;
  Table& _table;
};

}  // namespace opossum
