#ifndef SRC_THREADS_LOG_READER_H
#define SRC_THREADS_LOG_READER_H

#include <fstream>
#include <memory>
#include <string>

#include "log_record.h"

class LogReader
{
  public:
    explicit LogReader(const std::string &log_source = "/var/log/kern.log");

    std::list<std::shared_ptr<LogRecord>> read_logs();

  private:
    std::ifstream logFile;
};

#endif