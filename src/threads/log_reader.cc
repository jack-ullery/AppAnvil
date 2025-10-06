#include "log_reader.h"
#include "log_record.h"

#include <sstream>

LogReader::LogReader(const std::initializer_list<std::string> &log_sources)
{
  for (const std::string &source : log_sources) {
    log_files.emplace_front(source, std::ifstream::in);
  }
}

std::pair<std::list<std::shared_ptr<LogRecord>>, bool> LogReader::read_logs()
{
  std::list<std::shared_ptr<LogRecord>> logs;

  std::string log_data;
  for (std::ifstream &log_file : log_files) {
    while (log_file.is_open() && std::getline(log_file, log_data)) {
      auto log = std::make_shared<LogRecord>(log_data);

      if (log->valid()) {
        logs.push_back(log);
      }
    }
  }

  bool audit_success = append_audit_logs(logs);
  return { logs, audit_success };
}

bool LogReader::append_audit_logs(std::list<std::shared_ptr<LogRecord>> &log_list)
{
  auto results       = CommandCaller::get_logs(checkpoint_filepath);
  std::string output = results.first;
  bool read_success  = results.second;

  if (read_success) {
    std::istringstream stream(output);

    if (checkpoint_filepath.empty()) {
      std::getline(stream, checkpoint_filepath);
    }

    std::string line;
    while (std::getline(stream, line)) {
      auto log = std::make_shared<LogRecord>(line);

      if (log->valid()) {
        log_list.push_back(log);
      }
    }
  }

  return read_success || !log_list.empty();
}
