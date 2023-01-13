#include "log_record.h"
#include <aalogparse/aalogparse.h>

LogRecord::LogRecord(const std::string &log)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  char *log_char = const_cast<char *>(log.c_str());
  record_data    = parse_record(log_char);
}

LogRecord::~LogRecord()
{
  free_record(record_data);
}

bool LogRecord::valid() const
{
  return (record_data->event != AA_RECORD_INVALID) && (record_data->event != AA_RECORD_ERROR);
}

long LogRecord::timestamp() const
{
  return record_data->epoch;
}

aa_record_event_type LogRecord::event_type() const
{
  return record_data->event;
}

std::string LogRecord::event_type_string() const
{
  switch (record_data->event) {
    case AA_RECORD_INVALID:
      return "INVALID";

    case AA_RECORD_ERROR:
      return "ERROR";

    case AA_RECORD_AUDIT:
      return "AUDIT";

    case AA_RECORD_ALLOWED:
      return "ALLOWED";

    case AA_RECORD_DENIED:
      return "DENIED";

    case AA_RECORD_HINT:
      return "HINT";

    case AA_RECORD_STATUS:
      return "STATUS";
  }

  return "Unknown";
}

ulong LogRecord::pid() const
{
  return record_data->pid;
}

std::string LogRecord::operation() const
{
  return record_data->operation;
}

std::string LogRecord::profile() const
{
  return record_data->profile;
}

std::string LogRecord::name() const
{
  return record_data->name;
}

std::string LogRecord::requested_mask() const
{
  return record_data->requested_mask;
}

std::string LogRecord::denied_mask() const
{
  return record_data->denied_mask;
}
