#ifndef TABS_SWITCH_BOX_H
#define TABS_SWITCH_BOX_H

#include "switch_row.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>

class SwitchBox : public Gtk::VBox
{
public:
  SwitchBox();

private:
  std::list<std::shared_ptr<SwitchRow>> switch_list;
};

#endif
