#ifndef TABS_LOG_RECORD_H
#define TABS_LOG_RECORD_H

#include <string>

// We want to use aalogparse.h from the libapparmor-dev library (which was written in C)
// We need to mark this header with `extern "C"` so that it is compiled as C code and not C++
extern "C" {
    // Unfortunately the header contains the keyword: "namespace" as a variable name
    // Unlike C, "namespace" is a reserved keyword in C++
    // Therefore, we need to make a minor modification to the header file, so that our C++ code will compile.
    #define namespace namespace_variable    // NOLINT
    #include <aalogparse/aalogparse.h>
    #undef namespace
}

class LogRecord {
    public:
        // Main constructor
        explicit LogRecord(const std::string &log_str);

        // Destructor
        ~LogRecord();

        // Other special member functions (explicitly added to fix a warning)
        LogRecord(const LogRecord& source) noexcept = default;
        LogRecord(LogRecord&& source)      noexcept = default;
        LogRecord&  operator=(const LogRecord&)     = default;
        LogRecord&  operator=(LogRecord&&)          = default;

        // Method to access the parsed data (make sure to NOT free this data)
        // We may want to create getter functions in the future to hide this object
        const aa_log_record* Data();

    private:
        // The parsed record
        aa_log_record *data;
};

#endif // TABS_LOG_RECORD_H
