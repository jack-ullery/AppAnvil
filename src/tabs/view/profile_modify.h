#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <apparmor_parser.hh>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <list>
#include <memory>
#include <string>
#include <tuple>

class ProfileModify : public Gtk::ScrolledWindow
{
public:
  explicit ProfileModify(const std::string &profile_name);

protected:
  // Non-static helper functions
  void intialize_abstractions(const AppArmor::Parser &parsed_profile, const std::string &profile_name);

  // Static helper functions  
  static std::unordered_set<std::string> get_abstractions(const AppArmor::Parser &parsed_profile, const std::string &profile_name);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_title;
  std::unique_ptr<Gtk::Box>   m_abstraction_box;

  // Container of added abstractions
  std::map<std::string, std::shared_ptr<Gtk::Widget>> abstraction_map;
};

#endif
