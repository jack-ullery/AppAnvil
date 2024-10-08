#ifndef TABS_VIEW_ADD_ABSTRACTION_H
#define TABS_VIEW_ADD_ABSTRACTION_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/combobox.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/label.h>
#include <gtkmm/stack.h>
#include <gtkmm/textview.h>
#include <libappanvil/apparmor_parser.hh>
#include <memory>

class AddAbstraction
{
public:
  typedef std::pair<int, const AppArmor::AbstractionRule> show_dialog_response;

  /**
   * @brief Builds and shows the "Add Abstraction" dialog.
   *
   * @returns std::pair<int, AppArmor::Tree::AbstractionRule> The Gtk::ResponseType from the Dialog and the rule to add (if it exists)
   */
  static show_dialog_response show_dialog();

protected:
  AddAbstraction();

  // Signal Handlers
  void handle_file_button();
  void handle_entry_changed();
  void handle_button_cancel();
  void handle_button_next();
  void handle_button_back();
  void handle_button_accept();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets
  std::shared_ptr<Gtk::Dialog> dialog;
  std::unique_ptr<Gtk::HeaderBar> headerbar;
  std::unique_ptr<Gtk::Stack> stack_1;
  std::unique_ptr<Gtk::Stack> stack_2;

  std::unique_ptr<Gtk::Button> button_cancel;
  std::unique_ptr<Gtk::Button> button_next;
  std::unique_ptr<Gtk::Button> button_back;
  std::unique_ptr<Gtk::Button> button_accept;

  std::unique_ptr<Gtk::Entry> ab_entry;
  std::unique_ptr<Gtk::Button> ab_entry_button;
  std::unique_ptr<Gtk::TextView> ab_preview;
  std::unique_ptr<Gtk::TextView> ab_text;

  // Currently selected rule (if there is one)
  AppArmor::Tree::AbstractionRule prospective_rule;
};

#endif // TABS_VIEW_ADD_ABSTRACTION_H