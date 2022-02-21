#include "logs.h"

#include "jsoncpp/json/json.h"

#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::regex create_regex_from_tag(const std::string &tag) { return std::regex("\\b" + tag + "=\"([^ ]*)\""); }

// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
const std::regex filter_log_regex("audit: type=1400");                      // NOLINT(cert-err58-cpp)
const std::regex filter_log_time("\\[(.*)\\]");                             // NOLINT(cert-err58-cpp)
const std::regex filter_log_type      = create_regex_from_tag("apparmor");  // NOLINT(cert-err58-cpp)
const std::regex filter_log_operation = create_regex_from_tag("operation"); // NOLINT(cert-err58-cpp)
const std::regex filter_log_status    = create_regex_from_tag("profile");   // NOLINT(cert-err58-cpp)
const std::regex filter_log_name      = create_regex_from_tag("name");      // NOLINT(cert-err58-cpp)
const std::regex filter_log_pid("\\bpid=([0123456789]*)");                  // NOLINT(cert-err58-cpp)

std::string Logs::parse_line(const std::string &line, const std::regex &elem)
{
  std::smatch m;
  std::regex_search(line, m, elem);
  return m[1];
}

void Logs::add_row_from_line(const std::shared_ptr<StatusColumnRecord> &col_record, const std::string &line)
{
  auto row = col_record->new_row();
  col_record->set_row_data(row, 0, parse_line(line, filter_log_time) + '\t'); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  col_record->set_row_data(row, 1, parse_line(line, filter_log_type));        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  col_record->set_row_data(row, 2, parse_line(line, filter_log_operation));   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  col_record->set_row_data(row, 3, parse_line(line, filter_log_name));        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  col_record->set_row_data(row, 4, parse_line(line, filter_log_pid));         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  col_record->set_row_data(row, 5, parse_line(line, filter_log_status));      // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}

void Logs::add_data_to_record(const std::string &data)
{
  // Delete all the data from col_record
  col_record->clear();

  std::stringstream logs;
  logs << data;

  std::string line;

  while(std::getline(logs, line)) {
    if(std::regex_search(line, filter_log_regex)) {
      add_row_from_line(col_record, line);
    }
  }

  refresh();
}

void Logs::refresh()
{
  uint num_visible = col_record->filter_rows();
  Status::set_status_label_text(" " + std::to_string(num_visible) + " logs");
}

Logs::Logs() : col_record{StatusColumnRecord::create(Status::get_view(), col_names)}
{
  auto func = sigc::mem_fun(*this, &Logs::refresh);
  Status::set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &Logs::filter);
  col_record->set_visible_func(filter_fun);

  Status::remove_status_selection();

  this->show_all();
}
