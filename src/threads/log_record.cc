#include "log_record.h"

LogRecord::LogRecord(const std::string &log)
{
  char *log_char = const_cast<char*>(log.c_str());
  record_data = parse_record(log_char);
}

LogRecord::~LogRecord()
{
  free_record(record_data);
}