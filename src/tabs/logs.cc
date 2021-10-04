#include "jsoncpp/json/json.h"
#include "logs.h"

#include <iostream>
#include <string>
#include <vector>


std::regex create_regex_from_tag(std::string tag){
  return std::regex("\\b" + tag + "=\"([^ ]*)\""); 
}

const std::regex filter_log_regex("audit: type=1400");
const std::regex filter_log_type      = create_regex_from_tag("apparmor");
const std::regex filter_log_operation = create_regex_from_tag("operation");
const std::regex filter_log_profile   = create_regex_from_tag("profile");
const std::regex filter_log_name      = create_regex_from_tag("name");
const std::regex filter_log_pid("\\bpid=([0123456789]*)");

std::string Logs::parse_line(std::string line, std::regex elem){
  std::smatch m;
  std::regex_search(line, m, elem);
  return m[1];
}

void Logs::add_row_from_line(std::string line){
  auto row = *(list_store->append());
  row[col_record.column[0]] = parse_line(line, filter_log_type);
  row[col_record.column[1]] = parse_line(line, filter_log_operation);
  row[col_record.column[2]] = parse_line(line, filter_log_profile);
  row[col_record.column[3]] = parse_line(line, filter_log_name);
  row[col_record.column[4]] = parse_line(line, filter_log_pid);
}

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Logs::refresh(){
  
  list_store->clear();
  // Regex to filter logs from dmesg. Could also use "audit\\([1234567890:.]*\\)" to get more logs.
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

void Logs::order_columns(){
  // Notice the column retrieved is a TreeViewColumn, not a TreeModelColumn like was used with s_record
  // The column numbers depend on the order the are added to s_view
  for(uint i = 0; i < s_view->get_n_columns(); i++){
    auto *col = s_view->get_column(i);
    col->set_reorderable();
    col->set_resizable();
    col->set_min_width(MIN_COL_WIDTH);
    col->set_sort_column(col_record.column[i]);
  }
}

Logs::Logs()
:  list_store{Gtk::ListStore::create(col_record)}
{
  s_view->set_model(list_store);
  s_view->append_column("Type",       col_record.column[0]);
  s_view->append_column("Operation",  col_record.column[1]);
  s_view->append_column("Profile",    col_record.column[2]);
  s_view->append_column("Name",       col_record.column[3]);
  s_view->append_column("Pid",        col_record.column[4]);

  refresh();
  order_columns();

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