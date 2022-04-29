#ifndef TABS_VIEW_STATUS_H
#define TABS_VIEW_STATUS_H

#include "../column_header.h"
#include "../search_info.h"
#include "jsoncpp/json/json.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>
#include <memory>

constexpr auto UNKNOWN_STATUS = "unknown";

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
   * @brief Change the text in the label next to the Apply button/spinner.
   */
  void set_apply_label_text(const std::string &str);

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

  /**
   * @brief Set the method to be called every time the apply button is clicked.
   *
   * @details
   * Sets the signal handler method to be called every time the apply button is clicked. This
   * happens when the user clicks the 'Apply' button above the search bar.
   *
   * The signal handler should have the following protoype: `void method_name()`
   */
  void set_apply_signal_handler(const Glib::SignalProxyProperty::SlotType &func);

  /**
   * @brief Return the active selection/choice in the dropdown ComboBoxText
   *
   * @returns The string of the dropdown item which is selected.
   */
  Glib::ustring get_selection_text() const;

  SearchInfo get_search_info();

protected:

  /**
   * @brief Make widgets related to changing profile status invisible to the user.
   *
   * @details
   * Removes the widgets associated with changing a profiles status. This cannot be undone.
   * Widgets that are children of `s_selection_box` will be permanently invisible to the user
   * for this instance of Status.
   */
  void remove_status_selection();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Container Widgets
  std::shared_ptr<Gtk::TreeView> s_view;
  std::shared_ptr<Gtk::ScrolledWindow> s_win;
  std::unique_ptr<Gtk::Box> s_box;

  // Widgets related to searchbar
  std::unique_ptr<Gtk::SearchEntry> s_search;
  std::unique_ptr<Gtk::CheckButton> s_use_regex;
  std::unique_ptr<Gtk::CheckButton> s_match_case;
  std::unique_ptr<Gtk::CheckButton> s_whole_word;
  std::unique_ptr<Gtk::Label> s_found_label;

  // Widgets related to changing profile status (above search)
  std::unique_ptr<Gtk::Box> s_selection_box;
  std::unique_ptr<Gtk::Button> s_apply_button;
  std::unique_ptr<Gtk::Label> s_apply_info_text;
  std::unique_ptr<Gtk::ComboBoxText> s_status_selection;

  // Misc
  template<typename T_Widget> static std::shared_ptr<T_Widget> get_widget_shared(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
  template<typename T_Widget> static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);

  // clang-tidy complains about the `COLUMN_TYPE_STRING` macro, so we assign it here and tell clang-tidy not to look at it
  static constexpr unsigned int COLUMN_TYPE_STRING = G_TYPE_STRING; // NOLINT
};

#endif // TABS_VIEW_STATUS_H
