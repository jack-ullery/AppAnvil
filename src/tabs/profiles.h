#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "status.h"

#include <memory>
#include <string>
#include <vector>

class Profiles : public Status
{
  public:
    Profiles();
    void add_data_to_record(const std::string& data);
    void refresh();
    void change_status();
  
  protected:
    // Signal handler
    void on_apply_button_pressed();

  private:
    const std::vector<std::string> col_names{"Profile", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_PROFILES_H
