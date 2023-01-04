#include "profile_modify.h"
#include "../../threads/command_caller.h"

#include <apparmor_parser.hh>
#include <fstream>
#include <gtkmm/label.h>
#include <memory>
#include <pangomm/attributes.h>
#include <pangomm/attrlist.h>
#include <pangomm/fontdescription.h>
#include <pangomm/layout.h>

ProfileModify::ProfileModify(const std::string &profile_name)
  : m_box{ new Gtk::VBox() }
{
  //// Set values for the "header" widget ////
  header.set_label("Modify Profile");

  // Set the attributes for the "header" wiget
  Pango::AttrList attr;

  // Scale the header by 110%
  constexpr double HEADER_SCALE = 1.1;
  auto scale = Pango::Attribute::create_attr_scale(HEADER_SCALE);
  attr.insert(scale);

  // Make the header bold
  auto weight = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD);
  attr.insert(weight);

  header.set_attributes(attr);

  // Add the header to the main box
  m_box->add(header);

  // Add the main box to this widget
  m_box->set_halign(Gtk::ALIGN_START);
  m_box->set_valign(Gtk::ALIGN_START);

  this->add(*m_box);

  this->show_all();
}
