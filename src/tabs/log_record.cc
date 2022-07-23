#include "log_record.h"
#include <iostream>

LogRecord::LogRecord(const std::string &log)
{
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
    char *log_char_ptr = const_cast<char*>(log.data());
    
    // Parse the log into a record (which will need to be freed before the constructor completes)
    data = parse_record(log_char_ptr);

    // If the returned data is null, give an error message
    if(data == NULL) {
        std::cerr << "Could not parse the following record: " << log << std::endl;
    }
}

const aa_log_record* LogRecord::Data() {
    return data;
}


LogRecord::~LogRecord()
{
    if(data == NULL) {
        // Free the parsed record
        free_record(data);
    }
}