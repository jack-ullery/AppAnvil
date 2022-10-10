#include "log_record.h"
#include <cstddef>
#include <iostream>
#include <regex>

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex filter_log_regex("audit: type=1400"); // NOLINT(cert-err58-cpp)

LogRecord::LogRecord(const std::string &log)
{
    data = nullptr;
    if(std::regex_search(log, filter_log_regex)) {
        return;
    }

    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
    char *log_char = const_cast<char*>(log.c_str());

    // Parse the log into a record (which will need to be freed before the constructor completes)
    aa_log_record *record = parse_record(log_char);

    // If the returned data is null, give an error message
    if(record == nullptr) {
        std::cerr << "Could not parse the following record: " << log << std::endl;
    }
    else {
        data = record;
    }
}

bool LogRecord::has_data() {
    return (data != nullptr) && (data->pid != 0) && (data->epoch != 0);
}

aa_log_record* LogRecord::Data() {
    return data;
}

LogRecord::~LogRecord()
{
    if(data == nullptr) {
        // Free the parsed record
        free_record(data);
    }
}