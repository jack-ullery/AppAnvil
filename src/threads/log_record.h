#ifndef SRC_THREADS_LOG_RECORD_H
#define SRC_THREADS_LOG_RECORD_H

#include <list>
#include <string>

// We want to use aalogparse.h from the libapparmor-dev library (which was written in C)
// We need to mark this header with `extern "C"` so that it is compiled as C code and not C++
extern "C" {
  // Unfortunately the header contains the reserved keywords like "namespace" as a variable name
  // Unlike C, "namespace" is a reserved keyword in C++
  // Therefore, we need to make a minor modification to the header file, so that our C++ code will compile.
  #define namespace namespace_variable    // NOLINT
  #define class     class_variable        // NOLINT
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

    // Declares copy/move constructors and operators
    LogRecord(const LogRecord &)            = default;
    LogRecord(LogRecord &&)                 = default;
    LogRecord &operator=(const LogRecord &) = default;
    LogRecord &operator=(LogRecord &&)      = default;

  private: 
    aa_log_record *record_data;
};

#endif