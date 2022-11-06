#ifndef TABS_SWITCH_BOX_H
#define TABS_SWITCH_BOX_H

#include "switch_row.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>
#include <string>

// Tuple of constant strings, used as an argument for the SwitchBox
typedef std::tuple<const std::string, const std::string, const bool> string_tuple;

class SwitchBox : public Gtk::VBox
{
public:
  // Main constructor
  explicit SwitchBox(const std::vector<string_tuple> &tuples);

private:
  std::list<std::shared_ptr<SwitchRow>> switch_list;
};

#endif
