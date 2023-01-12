#include "log_reader.h"
#include "log_record.h"

LogReader::LogReader(const std::string &log_source)
  : logFile(log_source, std::ifstream::in)
{   }

std::list<LogRecord> LogReader::read_logs()
{
  std::list<LogRecord> logs;  

  std::string log_data;
  while (std::getline(logFile, log_data)) {
    if(log_data.find("audit: type=1400") != std::string::npos) {
        LogRecord log(log_data);
        logs.push_back(log);
    }
  }

  return logs;
}