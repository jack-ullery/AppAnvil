#include "jsoncpp/json/json.h"
#include "logs.h"

#include <iostream>
#include <string>
#include <vector>


std::regex create_regex_from_tag(const std::string& tag){
  return std::regex("\\b" + tag + "=\"([^ ]*)\"");
}

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex filter_log_regex("audit: type=1400");                      // NOLINT(cert-err58-cpp)
const std::regex filter_log_type      = create_regex_from_tag("apparmor");  // NOLINT(cert-err58-cpp)
const std::regex filter_log_operation = create_regex_from_tag("operation"); // NOLINT(cert-err58-cpp)
const std::regex filter_log_status    = create_regex_from_tag("profile");   // NOLINT(cert-err58-cpp)
const std::regex filter_log_name      = create_regex_from_tag("name");      // NOLINT(cert-err58-cpp)
const std::regex filter_log_pid("\\bpid=([0123456789]*)");                  // NOLINT(cert-err58-cpp)


std::string Logs::parse_line(const std::string& line, const std::regex& elem){
  std::smatch m;
  std::regex_search(line, m, elem);
  return m[1];
}

void Logs::add_row_from_line(const std::string& line){
  auto row = col_record->new_row();
  col_record->set_row_data(row, 0, parse_line(line, filter_log_type));
  col_record->set_row_data(row, 1, parse_line(line, filter_log_operation)); 
  col_record->set_row_data(row, 2, parse_line(line, filter_log_name));
  col_record->set_row_data(row, 3, parse_line(line, filter_log_pid));
  col_record->set_row_data(row, 4, parse_line(line, filter_log_status)); 
}

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Logs::refresh(){
  
  col_record->clear();
  // Regex to filter logs from dmesg. Could also use as filter_log_regex "audit\\([1234567890:.]*\\)" to get more logs.
  int num_found = 0;

  std::stringstream logs;
  logs << Status::get_logs_str();

  std::string line;
  while(std::getline(logs, line)){
    if(std::regex_search(line, filter_log_regex) && filter(line)){
      add_row_from_line(line);
      num_found++;
    }
  }

  Status::set_status_label_text(" " + std::to_string(num_found) + " logs");
}

Logs::Logs()
: col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  refresh();

  auto func = sigc::mem_fun(*this, &Logs::on_search_changed);
  Status::set_signal_handler(func);

  this->show_all();
}

void Logs::on_search_changed(){
  refresh();
}