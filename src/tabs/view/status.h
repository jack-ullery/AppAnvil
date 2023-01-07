#ifndef TABS_VIEW_STATUS_H
#define TABS_VIEW_STATUS_H

#include "../column_header.h"
#include "../search_info.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>
#include <memory>

class Status : public Gtk::ScrolledWindow
{
public:
  Status();

  /**
   * @brief Return the TreeView associated with this class.
   *
   * @returns The TreeView data member used by this class.
   */
  std::shared_ptr<Gtk::TreeView> get_view();

  /**
   * @brief Return the ScrolledWindow associated with this class.
   *
   * @returns The ScrolledWindow data member used by this class.
   */
  std::shared_ptr<Gtk::ScrolledWindow> get_window();

  /**
   * @brief Change the text in the label directly above the searchbar.
   */
  void set_status_label_text(const std::string &str);

  /**
   * @brief Set the method to be called every time the search area updated.
   *
   * @details
   * Sets the signal handler method to be called every time the search area is updated. This
   * happens when the user clicks a checkbutton, or types into the searchbar.
   *
   * The signal handler should have the following protoype: `void method_name()`
   */
  void set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType &func);

  SearchInfo get_search_info();
  void hide_searchbar();
  void show_searchbar(const bool &should_focus);

protected:
  explicit Status(const std::string &glade_resource);

  std::shared_ptr<Gtk::Builder> get_builder();

private:
  // GUI Builder to parse UI from xml file
  std::shared_ptr<Gtk::Builder> builder;

  // Container Widgets
  std::shared_ptr<Gtk::TreeView> s_view;
  std::shared_ptr<Gtk::ScrolledWindow> s_win;
  std::unique_ptr<Gtk::Box> s_box;

  // Widgets related to searchbar
  std::unique_ptr<Gtk::Box> s_searchbox;
  std::unique_ptr<Gtk::SearchEntry> s_search;
  std::unique_ptr<Gtk::CheckButton> s_use_regex;
  std::unique_ptr<Gtk::CheckButton> s_match_case;
  std::unique_ptr<Gtk::CheckButton> s_whole_word;
  std::unique_ptr<Gtk::Label> s_found_label;

  // clang-tidy complains about the `COLUMN_TYPE_STRING` macro, so we assign it here and tell clang-tidy not to look at it
  static constexpr unsigned int COLUMN_TYPE_STRING = G_TYPE_STRING; // NOLINT
};

#endif // TABS_VIEW_STATUS_H
