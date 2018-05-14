#pragma once

#include "types.hpp"

namespace opossum {

/*
  Existence of logfile is only checked on instantiation. Therefore deleting the log during runtime is undetected.
*/

class AbstractLogger {
 public:  
  AbstractLogger(const AbstractLogger&) = delete;
  AbstractLogger& operator=(const AbstractLogger&) = delete;

  virtual void commit(const TransactionID transaction_id) = 0;

  virtual void value(const TransactionID transaction_id, const std::string table_name, const RowID row_id, const std::stringstream &values) = 0;

  virtual void invalidate(const TransactionID transaction_id, const std::string table_name, const RowID row_id) = 0;

  void flush();

  virtual ~AbstractLogger() = default;

 protected:
  void _write_to_logfile(const std::stringstream& ss);

  int _file_descriptor;
  std::mutex _mutex;

 private:
  friend class Logger;
  friend class InitialLogger;
  AbstractLogger();
};

}  // namespace opossum