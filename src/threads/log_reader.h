#ifndef SRC_THREADS_LOG_READER_H
#define SRC_THREADS_LOG_READER_H

#include <forward_list>
#include <fstream>
#include <memory>
#include <string>

#include "log_record.h"

class LogReader
{
public:
  LogReader(const std::initializer_list<std::string> &log_sources = { "/var/log/kern.log", "/var/log/dmesg", "/var/log/syslog" });

  std::list<std::shared_ptr<LogRecord>> read_logs();

protected:
  // Append logs from ausearch to list
  void append_audit_logs(std::list<std::shared_ptr<LogRecord>> &log_list);

private:
  std::forward_list<std::ifstream> log_files;
  std::string checkpoint_filepath;
};

#endif