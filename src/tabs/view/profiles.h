#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "status.h"

#include <memory>
#include <string>
#include <vector>

class Profiles : public Status
{
public:
  explicit Profiles();

  // Sets the function to be used when changing the status of a profile, this is used in main_window.cc
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

  /**
   * @brief Change the text in the label next to the Apply button/spinner.
   */
  void set_apply_label_text(const std::string &str);

  void set_profile_info(const std::string &status, const std::string &num_perms, const std::string &num_procs);

  void show_profile_info();
  void hide_profile_info();

protected:
  // Signal handler to handle when the user wants to change the status of a profile
  // This calls the default_change_fun with the correct values for the profile, old_status, and new_status
  void change_status();

private:
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;

  // Widgets
  Glib::RefPtr<Gtk::Builder> builder;
  
  std::shared_ptr<Gtk::ComboBoxText> p_status_selection;
  std::shared_ptr<Gtk::Button> p_apply_button;
  std::shared_ptr<Gtk::Label> p_apply_info_text;

  std::shared_ptr<Gtk::Box> p_profile_info;
  std::shared_ptr<Gtk::Label> p_status_label;
  std::shared_ptr<Gtk::Label> p_num_proc_label;
  std::shared_ptr<Gtk::Label> p_num_perm_label;

  // Misc
  template<typename T_Widget> static std::shared_ptr<T_Widget> get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // TABS_PROFILES_H
