#ifndef SRC_THREADS_LOG_READER_H
#define SRC_THREADS_LOG_READER_H

#include "log_record.h"
#include <string>
#include <fstream>

class LogReader
{
  public:
    explicit LogReader(const std::string &log_source = "/var/log/kern.log");

    std::list<LogRecord> read_logs();

  private:
    std::ifstream logFile;
};

#endif