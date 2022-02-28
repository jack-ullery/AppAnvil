#ifndef TABS_LOGS_H
#define TABS_LOGS_H

#include "status.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>

template<class ColumnRecord>
class Logs : public Status
{
public:
  Logs();

  // For unit testing
  explicit Logs(std::shared_ptr<ColumnRecord> col_record);

  virtual void add_data_to_record(const std::string &data);
  static void add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line);
  static std::string parse_line(const std::string &line, const std::regex &elem);
  void refresh();

private:
  const std::vector<std::string> col_names{"Time", "Type", "Operation", "Name", "Pid", "Status"};
  std::shared_ptr<ColumnRecord> col_record;
};

  //std::regex create_regex_from_tag(const std::string &tag);
  template<class ColumnRecord>
  std::regex create_regex_from_tag(const std::string &tag) { return std::regex("\\b" + tag + "=\"([^ ]*)\""); }

  // clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
  const std::regex filter_log_regex("audit: type=1400");									// NOLINT(cert-err58-cpp)
  const std::regex filter_log_time("\\[(.*)\\]");											// NOLINT(cert-err58-cpp)
  template<class ColumnRecord> 
  const std::regex filter_log_type = create_regex_from_tag<ColumnRecord>("apparmor");		// NOLINT(cert-err58-cpp)
  template<class ColumnRecord> 
  const std::regex filter_log_operation = create_regex_from_tag<ColumnRecord>("operation"); // NOLINT(cert-err58-cpp)
  template<class ColumnRecord> 
  const std::regex filter_log_status = create_regex_from_tag<ColumnRecord>("profile");		// NOLINT(cert-err58-cpp)
  template<class ColumnRecord> 
  const std::regex filter_log_name   = create_regex_from_tag<ColumnRecord>("name");			// NOLINT(cert-err58-cpp)
  const std::regex filter_log_pid("\\bpid=([0123456789]*)");								// NOLINT(cert-err58-cpp)

#include "logs.inl"

#endif // TABS_LOGS_H
