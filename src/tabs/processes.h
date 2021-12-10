#ifndef TABS_PROCESSES_H
#define TABS_PROCESSES_H

#include "status.h"

#include <memory>
#include <regex>
#include <string>

class Processes : public Status
{
  public:
    Processes();
    void add_data_to_record(const std::string& confined, const std::string& unconfined);
    void refresh();

  protected:
    // Signal handlers    
    void on_search_changed();

  private:
    const std::vector<std::string> col_names{"Process"};
    std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_PROCESSES_H
