#include "jsoncpp/json/json.h"
#include "logs.h"

#include <iostream>
#include <string>
#include <vector>


std::regex create_regex_from_tag(const std::string& tag){
  return std::regex("\\b" + tag + "=\"([^ ]*)\""); 
}

const std::regex filter_log_regex("audit: type=1400");
const std::regex filter_log_type      = create_regex_from_tag("apparmor");
const std::regex filter_log_operation = create_regex_from_tag("operation");
const std::regex filter_log_status    = create_regex_from_tag("profile");
const std::regex filter_log_name      = create_regex_from_tag("name");
const std::regex filter_log_pid("\\bpid=([0123456789]*)");

std::string Logs::parse_line(const std::string& line, const std::regex& elem){
  std::smatch m;
  std::regex_search(line, m, elem);
  return m[1];
}

void Logs::add_row_from_line(const std::string& line){
  auto row = col_record->new_column();
  row[col_record->column[0]] = parse_line(line, filter_log_type);
  row[col_record->column[1]] = parse_line(line, filter_log_operation);
  row[col_record->column[2]] = parse_line(line, filter_log_name);
  row[col_record->column[3]] = parse_line(line, filter_log_pid);
  row[col_record->column[4]] = parse_line(line, filter_log_status);
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

  s_found_label->set_text(" " + std::to_string(num_found) + " logs");
}

Logs::Logs()
: col_record{StatusColumnRecord::create(s_view, col_names)}
{
  refresh();

  auto sig_handler = sigc::mem_fun(*this, &Logs::on_search_changed);
  s_search->signal_search_changed().connect(sig_handler, true);
  s_use_regex->signal_clicked().connect(sig_handler, true);
  s_match_case->signal_clicked().connect(sig_handler, true);
  s_whole_word->signal_clicked().connect(sig_handler, true);

  this->show_all();
}

void Logs::on_search_changed(){
  refresh();
}