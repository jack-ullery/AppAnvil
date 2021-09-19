#ifndef GTKMM_EXAMPLE_STATUS_H
#define GTKMM_EXAMPLE_STATUS_H

#include "jsoncpp/json/json.h"
#include <sstream>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>

class Status : public Gtk::ScrolledWindow
{
  public:
    Status();
    static std::string get_status_str();
    static Json::Value get_status_JSON();

  protected:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Container Widgets
    std::unique_ptr<Gtk::TreeView> s_view;
    std::unique_ptr<Gtk::ScrolledWindow> s_win;
    std::unique_ptr<Gtk::Box> s_box;

    // Widgets related to searchbar
    std::unique_ptr<Gtk::SearchEntry> s_search;
    std::unique_ptr<Gtk::CheckButton> s_use_regex;
    std::unique_ptr<Gtk::CheckButton> s_match_case;
    std::unique_ptr<Gtk::CheckButton> s_whole_word;
    std::unique_ptr<Gtk::Label>       s_found_label;

    bool filter(const std::string& str);

  private:
    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(const Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);    
    static Json::Value parse_JSON(const std::string& raw_json);
    static bool filter(const std::string& str, const std::string& rule, const bool& use_regex, const bool& match_case, const bool& whole_word);
};

#endif // GTKMM_EXAMPLE_STATUS_H
