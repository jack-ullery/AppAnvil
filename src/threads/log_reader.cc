#include "log_reader.h"
#include "log_record.h"

LogReader::LogReader(const std::string &log_source)
  : logFile(log_source, std::ifstream::in)
{   }

std::list<std::shared_ptr<LogRecord>> LogReader::read_logs()
{
  std::list<std::shared_ptr<LogRecord>> logs;  

  std::string log_data;
  while (std::getline(logFile, log_data)) {
    auto log = std::make_shared<LogRecord>(log_data);

    if(log->valid()) {
      logs.push_back(log);
    }
  }

  return logs;
}