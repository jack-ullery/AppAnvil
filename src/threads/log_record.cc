#include "log_record.h"
#include <aalogparse/aalogparse.h>

LogRecord::LogRecord(const std::string &log)
{
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  char *log_char = const_cast<char *>(log.c_str());
  record_data    = parse_record(log_char);
}

LogRecord::~LogRecord()
{
  free_record(record_data);
}

bool LogRecord::valid() const
{
  return (record_data->event != AA_RECORD_INVALID) && (record_data->event != AA_RECORD_ERROR);
}

long LogRecord::timestamp() const
{
  return record_data->epoch;
}

aa_record_event_type LogRecord::event_type() const
{
  return record_data->event;
}

std::string LogRecord::event_type_string() const
{
  switch (record_data->event) {
    case AA_RECORD_INVALID:
      return "INVALID";

    case AA_RECORD_ERROR:
      return "ERROR";

    case AA_RECORD_AUDIT:
      return "AUDIT";

    case AA_RECORD_ALLOWED:
      return "ALLOWED";

    case AA_RECORD_DENIED:
      return "DENIED";

    case AA_RECORD_HINT:
      return "HINT";

    case AA_RECORD_STATUS:
      return "STATUS";
  }

  return "Unknown";
}

ulong LogRecord::pid() const
{
  return record_data->pid;
}

// Converts the parameter to a string (or an empty string if it is a nullptr)
std::string convert_to_string(const char *ptr)
{
  if (ptr == nullptr) {
    return "";
  }
  return std::string(ptr);
}

std::string LogRecord::operation() const
{
  return convert_to_string(record_data->operation);
}

std::string LogRecord::profile() const
{
  return convert_to_string(record_data->profile);
}

std::string LogRecord::name() const
{
  return convert_to_string(record_data->name);
}

// Attempts to emplace key/value pair to a list (if the 'value' is not null)
void attempt_emplace_pair(std::list<std::pair<std::string, std::string>> &metadata, const std::string &key, const char *value)
{
  if (value != nullptr) {
    metadata.emplace_back(key, value);
  }
}

std::list<std::pair<std::string, std::string>> LogRecord::get_metadata() const
{
  std::list<std::pair<std::string, std::string>> metadata;
  attempt_emplace_pair(metadata, "Audit ID", record_data->audit_id);
  attempt_emplace_pair(metadata, "Denied Mask", record_data->denied_mask);
  attempt_emplace_pair(metadata, "Requested Mask", record_data->requested_mask);
  attempt_emplace_pair(metadata, "Peer Profile", record_data->peer_profile);
  attempt_emplace_pair(metadata, "Comm", record_data->comm);
  attempt_emplace_pair(metadata, "Namespace", record_data->aa_namespace);
  attempt_emplace_pair(metadata, "Attribute", record_data->attribute);
  attempt_emplace_pair(metadata, "Info", record_data->info);
  attempt_emplace_pair(metadata, "Peer Info", record_data->peer_info);
  attempt_emplace_pair(metadata, "Active Hat", record_data->active_hat);
  attempt_emplace_pair(metadata, "Net Family", record_data->net_family);
  attempt_emplace_pair(metadata, "Net Protocol", record_data->net_protocol);
  attempt_emplace_pair(metadata, "Net Sock Type", record_data->net_sock_type);
  attempt_emplace_pair(metadata, "Net Local Address", record_data->net_local_addr);
  attempt_emplace_pair(metadata, "Net Foreign Address", record_data->net_foreign_addr);
  attempt_emplace_pair(metadata, "Dbus Bus", record_data->dbus_bus);
  attempt_emplace_pair(metadata, "Dbus Path", record_data->dbus_path);
  attempt_emplace_pair(metadata, "Dbus Interface", record_data->dbus_interface);
  attempt_emplace_pair(metadata, "Dbus Member", record_data->dbus_member);
  attempt_emplace_pair(metadata, "Signal", record_data->signal);
  attempt_emplace_pair(metadata, "Peer", record_data->peer);
  attempt_emplace_pair(metadata, "File System Type", record_data->fs_type);
  attempt_emplace_pair(metadata, "Flags", record_data->flags);
  attempt_emplace_pair(metadata, "Source Name", record_data->src_name);
  // attempt_emplace_pair(metadata, "Class", record_data->rule_class);
  return metadata;
}
