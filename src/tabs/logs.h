#ifndef TABS_LOGS_H
#define TABS_LOGS_H

#include "status.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>

template<class ColumnRecord> class Logs : public Status
{
public:
  // For production
  Logs();
  // For unit testing
  explicit Logs(std::shared_ptr<ColumnRecord> column_record);

  virtual void add_data_to_record(const std::string &data);
  static void add_row_from_line(const std::shared_ptr<ColumnRecord> &col_record, const std::string &line);
  static std::string parse_line(const std::string &line, const std::regex &elem);
  void refresh();

private:
  const std::vector<ColumnHeader> col_names{ColumnHeader("Time"),
                                            ColumnHeader("Type"),
                                            ColumnHeader("Operation"),
                                            ColumnHeader("Name"),
                                            ColumnHeader("Pid", ColumnHeader::ColumnType::INT),
                                            ColumnHeader("Status")};
  std::shared_ptr<ColumnRecord> col_record;
};

// Simple wrappers around a std::regex constructor
static std::regex create_regex(const std::string &regex) { return std::regex(regex); }
static std::regex create_regex_from_tag(const std::string &tag) { return std::regex("\\b" + tag + "=\"([^ ]*)\""); }

// clang-format off
// clang-tidy throws [cert-err58-cpp], but it's not a problem in this case, so lets ignore it.
static const std::regex filter_log_regex = create_regex("audit: type=1400");       // NOLINT(cert-err58-cpp)
static const std::regex filter_log_time = create_regex("\\[(.*)\\]");              // NOLINT(cert-err58-cpp)
static const std::regex filter_log_pid = create_regex("\\bpid=([0123456789]*)");   // NOLINT(cert-err58-cpp)
static const std::regex filter_log_type = create_regex_from_tag("apparmor");       // NOLINT(cert-err58-cpp)
static const std::regex filter_log_operation = create_regex_from_tag("operation"); // NOLINT(cert-err58-cpp)
static const std::regex filter_log_status = create_regex_from_tag("profile");      // NOLINT(cert-err58-cpp)
static const std::regex filter_log_name = create_regex_from_tag("name");           // NOLINT(cert-err58-cpp)
// clang-format on

#include "logs.inl"

#endif // TABS_LOGS_H
