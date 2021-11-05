#ifndef TABS_FILECHOOSER_H
#define TABS_FILECHOOSER_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/enums.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <memory>
#include <string>
#include <vector>

class FileChooser : public Status
{
  public:
    FileChooser();
    void add_data_to_record(std::string data);
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
