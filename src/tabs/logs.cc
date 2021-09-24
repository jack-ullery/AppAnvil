#include "jsoncpp/json/json.h"
#include "logs.h"

#include <iostream>
#include <regex>
#include <string>
#include <vector>

// refresh() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Logs::refresh(){
  
  list_store->clear();
  // Regex to filter logs from dmesg. Could also use "audit\\([1234567890:.]*\\)" to get more logs.
  std::regex log_filter_regex("audit: type=1400");
  int num_found = 0;

  std::stringstream logs;
  logs << Status::get_logs_str();

  std::string line;
  while(std::getline(logs, line)){
    if(std::regex_search(line, log_filter_regex) && filter(line)){
      auto row = *(list_store->append());
      row[col_record.log_col] = line;
      num_found++;
    }
  }

  s_found_label->set_text(" " + std::to_string(num_found) + " logs");
}

void Logs::order_columns(){
  // Notice the column retrieved is a TreeViewColumn, not a TreeModelColumn like was used with s_record
  // The column numbers depend on the order the are added to s_view
  auto *profile_view_col = s_view->get_column(0);
  profile_view_col->set_reorderable();
  profile_view_col->set_resizable();
  profile_view_col->set_min_width(MIN_COL_WIDTH);
  profile_view_col->set_sort_column(col_record.log_col);
}

Logs::Logs()
:  list_store{Gtk::ListStore::create(col_record)}
{
  s_view->set_model(list_store);
  s_view->append_column("Unparsed Logs", col_record.log_col);

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