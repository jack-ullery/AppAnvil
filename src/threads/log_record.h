#ifndef SRC_THREADS_LOG_RECORD_H
#define SRC_THREADS_LOG_RECORD_H

#include <forward_list>
#include <list>
#include <string>

// We want to use aalogparse.h from the libapparmor-dev library (which was written in C)
// We need to mark this header with `extern "C"` so that it is compiled as C code and not C++
extern "C"
{
// Unfortunately the header contains the reserved keywords like "namespace" as a variable name
// Unlike C, "namespace" is a reserved keyword in C++
// Therefore, we need to make a minor modification to the header file, so that our C++ code will compile.
#define namespace aa_namespace // NOLINT
#define class class_variable   // NOLINT
#include <aalogparse/aalogparse.h>
#undef class
#undef namespace
}

class LogRecord
{
public:
  // Initializes this class based on the data from a single log
  explicit LogRecord(const std::string &log);

  // Custom destructor. Calls free_record() on record_data
  ~LogRecord();

  // Whether or not this log is a valid AppArmor log that was parsed correctly, not some other type of log
  bool valid() const;

  // Accessors - these methods call record_data directly
  long timestamp() const;

  aa_record_event_type event_type() const;
  std::string event_type_string() const;

  ulong pid() const;

  std::string operation() const;
  std::string profile() const;
  std::string name() const;

  // This method returns a list of additional log information that cannot be otherwise queried from the above accessors
  std::list<std::pair<std::string, std::string>> get_metadata() const;

  // Deletes copy/move constructors and operators
  LogRecord(const LogRecord &)            = delete;
  LogRecord(LogRecord &&)                 = delete;
  LogRecord &operator=(const LogRecord &) = delete;
  LogRecord &operator=(LogRecord &&)      = delete;

private:
  aa_log_record *record_data;
};

#endif