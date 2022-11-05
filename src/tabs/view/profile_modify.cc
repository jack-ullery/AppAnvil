#include "profile_modify.h"
#include "switch_box.h"
#include "switch_row.h"

#include <gtkmm/label.h>
#include <memory>

ProfileModify::ProfileModify()
  : m_box{ new Gtk::VBox() }
{
  header.set_text("Header");
  m_box->add(header);

  std::shared_ptr<SwitchBox> ptr(new SwitchBox());
  switch_box_list.push_back(ptr);
  m_box->add(*ptr);

  this->add(*m_box);
}
