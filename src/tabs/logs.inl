template<class ColumnRecord> std::string Logs<ColumnRecord>::parse_line(const std::string &line, const std::regex &elem)
{
  std::smatch m;
  std::regex_search(line, m, elem);
  return m[1];
}

template<class ColumnRecord>
void Logs<ColumnRecord>::add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line)
{
  auto row = col_record->new_row();
  // clang-format off
  row->set_value(0, parse_line(line, filter_log_time) + '\t'); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(1, parse_line(line, filter_log_type));        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(2, parse_line(line, filter_log_operation));   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(3, parse_line(line, filter_log_name));        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(4, stoul(parse_line(line, filter_log_pid)));  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(5, parse_line(line, filter_log_status));      // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // clang-format on
}

template<class ColumnRecord> void Logs<ColumnRecord>::add_data_to_record(const std::string &data)
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

template<class ColumnRecord> void Logs<ColumnRecord>::refresh()
{
  uint num_visible = col_record->filter_rows();
  Status::set_status_label_text(" " + std::to_string(num_visible) + " logs");
}

// For unit testing
template<class ColumnRecord>
Logs<ColumnRecord>::Logs(std::shared_ptr<ColumnRecord> column_record) : col_record{std::move(column_record)} { }

// For production
template<class ColumnRecord> Logs<ColumnRecord>::Logs() : col_record{ColumnRecord::create(Status::get_view(), col_names)}
{
  auto func = sigc::mem_fun(*this, &Logs::refresh);
  Status::set_refresh_signal_handler(func);

  auto filter_fun = sigc::mem_fun(*this, &Logs::filter);
  col_record->set_visible_func(filter_fun);

  Status::remove_status_selection();

  this->show_all();
}
