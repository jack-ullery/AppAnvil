#include "database.h"
#include <exception>
#include <gtkmm/treeiter.h>
#include <memory>

// void Database::put_process_data(const std::string &profile_name, const unsigned int &pid)
// {
//     // Not implemented yet!
//     std::ignore = profile_name;
//     std::ignore = pid;
// }

// void Database::put_log_data(const std::string &profile_name, 
//                             const unsigned int &pid, 
//                             const std::string &timestamp,
//                             const std::string &type,
//                             const std::string &operation)
// {
//     // Not implemented yet!
//     std::ignore = profile_name;
//     std::ignore = pid;
//     std::ignore = timestamp;
//     std::ignore = type;
//     std::ignore = operation;
// }

// void Database::register_profile_column_record(const std::shared_ptr<StatusColumnRecord> &col_record)
// {
//     if(col_record == nullptr) {
//         throw std::runtime_error("Error: Argument for ColumnRecord (Profile) is null.");
//     } 
    
//     if (prof_col_record != nullptr) {
//         throw std::runtime_error("Error: Attempted to initialize ColumnRecord (Profile) more than once.");
//     }

//     prof_col_record = col_record;
// }
