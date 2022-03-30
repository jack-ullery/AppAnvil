template<class ColumnRecord> std::string Logs<ColumnRecord>::format_log_data(std::string &data)
{
  data.erase(std::remove(data.begin(), data.end(), '\"'), data.end());
  return data;
}

template<class ColumnRecord> std::string Logs<ColumnRecord>::format_timestamp(const time_t &timestamp)
{
  struct tm *timeInfo = localtime(&timestamp);
  std::string timeString = asctime(timeInfo);
  timeString.erase(std::remove(timeString.begin(), timeString.end(), '\n'), timeString.end());
  return timeString + '\t';
}

template<class ColumnRecord>
void Logs<ColumnRecord>::add_row_from_json(const std::shared_ptr<ColumnRecord> &col_record, const Json::Value &entry)
{
  auto row = col_record->new_row();

  time_t timestamp = (const time_t) (std::stol(entry["_SOURCE_REALTIME_TIMESTAMP"].asString()) / 1000000);
  LogData row_data(timestamp, entry["_AUDIT_FIELD_APPARMOR"].asString(), entry["_AUDIT_FIELD_OPERATION"].asString(),
                   entry["_AUDIT_FIELD_NAME"].asString(), entry["_PID"].asString(), entry["_AUDIT_FIELD_PROFILE"].asString());

  // clang-format off
  row->set_value(0, format_timestamp(row_data.timestamp));  // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(1, format_log_data(row_data.type));        // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(2, format_log_data(row_data.operation));   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(3, row_data.name);                         // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(4, stoul(row_data.pid));                   // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  row->set_value(5, format_log_data(row_data.status));      // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // clang-format on
}

template<class ColumnRecord> void Logs<ColumnRecord>::add_data_to_record(const std::string &data)
{
  // Delete all the data from col_record
  col_record->clear();

  std::stringstream logs;
  std::istringstream json_data(data);
  std::string line;
  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;

  while(std::getline(json_data, line)) {
    logs << line;
    if(!parseFromStream(builder, logs, &root, &errs)) {
      throw std::invalid_argument(errs + "\nArgument of add_data_to_record contains line with invalid JSON format.");
    }
    add_row_from_json(col_record, root);
    logs.clear();
    root.clear();
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