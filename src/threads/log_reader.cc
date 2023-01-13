#include "log_reader.h"
#include "log_record.h"

LogReader::LogReader(const std::initializer_list<std::string> &log_sources)
{
  for (const std::string &source : log_sources) {
    log_files.emplace_front(source, std::ifstream::in);
  }
}

std::list<std::shared_ptr<LogRecord>> LogReader::read_logs()
{
  std::list<std::shared_ptr<LogRecord>> logs;

  std::string log_data;
  for (std::ifstream &log_file : log_files) {
    while (std::getline(log_file, log_data)) {
      auto log = std::make_shared<LogRecord>(log_data);

      if (log->valid()) {
        logs.push_back(log);
      }
    }
  }

  return logs;
}