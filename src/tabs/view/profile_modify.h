#ifndef TABS_PROFILE_MODIFY_H
#define TABS_PROFILE_MODIFY_H

#include <apparmor_file_rule.hh>
#include <apparmor_parser.hh>
#include <apparmor_profile.hh>
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
  // Static helper functions  
  /**
   *  Attempt to find a the profile that corresponds to "profile_name" from the parsed profiles.
   *  Returns a reference to a nullptr if no profile was found.
   */
  static std::unique_ptr<AppArmor::Profile> find_profile(const AppArmor::Parser &parsed_profile, const std::string &profile_name);

  // Non-static helper functions
  void intialize_abstractions(const std::unique_ptr<AppArmor::Profile> &profile);
  void intialize_file_rules(const std::unique_ptr<AppArmor::Profile> &profile);

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // VBox which holds all the widgets
  std::unique_ptr<Gtk::Box> m_box;

  // Widgets
  std::unique_ptr<Gtk::Label> m_title;
  std::unique_ptr<Gtk::Box>   m_abstraction_box;
  std::unique_ptr<Gtk::Box>   m_file_rule_box;

  // Container of added abstractions
  std::map<std::string, std::shared_ptr<Gtk::Widget>> abstraction_map;

  // Container of added file rules
  std::map<std::shared_ptr<AppArmor::FileRule>, std::shared_ptr<Gtk::Widget>> file_rule_map;
};

#endif
