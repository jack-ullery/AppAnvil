#include "log_record.h"
#include <iostream>
#include <ostream>

LogRecord::LogRecord(const std::string &log)
{
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
    char *log_char = const_cast<char*>(log.c_str());

    // Parse the log into a record (which will need to be freed before the constructor completes)
    aa_log_record *record = parse_record(log_char);

    // If the returned data is null, give an error message
    if(record == NULL) {
        std::cerr << "Could not parse the following record: " << log << std::endl;
    }
    else {
        data = record;
    }
}

bool LogRecord::has_data() {
    return (data != NULL);
}

aa_log_record* LogRecord::Data() {
    return data;
}

LogRecord::~LogRecord()
{
    if(data == NULL) {
        // Free the parsed record
        free_record(data);
    }
}