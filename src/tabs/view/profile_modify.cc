#include "common.h"
#include "profile_modify.h"
#include "../../threads/command_caller.h"

#include <apparmor_parser.hh>

ProfileModify::ProfileModify(const std::string &profile_name)
  : builder{ Gtk::Builder::create_from_resource("/resources/profile_modify.glade") },
    m_box{ Common::get_widget<Gtk::Box>("m_box", builder) }
{
  this->add(*m_box);
  this->show_all();
}
