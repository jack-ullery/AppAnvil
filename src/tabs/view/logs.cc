#include "logs.h"
#include "common.h"
#include "info_box.h"

void Logs::set_information(const std::list<std::pair<std::string, std::string>> &data)
{
  uint index = 0;
  for (const auto &pair : data) {
    // If needed, create a new InfoBox to show this pair
    if (index >= info_vec.size()) {
      info_vec.emplace_back();
      l_log_info->append(info_vec[index]);
    }

    // Set the header and text of the next Info Box using data from the pair
    info_vec[index].set_text(pair.first, pair.second);
    info_vec[index].set_visible(true);

    index++;
  }

  // Set the uninitialized info boxes to be invisible
  while (index < info_vec.size()) {
    info_vec[index].set_visible(false);
    index++;
  }
}

Logs::Logs()
  : Status("/resources/log.ui"),
    builder{ Status::get_builder() },
    l_log_info{ Common::get_widget<Gtk::Box>("l_log_info", builder) }
{   }
