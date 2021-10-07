#ifndef TABS_STATUS_H
#define TABS_STATUS_H

#include "jsoncpp/json/json.h"
#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <sstream>

constexpr auto UNKNOWN_STATUS = "unknown";

class Status : public Gtk::ScrolledWindow
{
  public:
    Status();
    static std::string get_status_str();
    static Json::Value get_status_JSON();
    static std::string get_logs_str();
    static std::string get_unconfined();

  protected:
    bool filter(const std::string& str);
    void set_status_label_text(const std::string& str);
    void set_signal_handler(const Glib::SignalProxyProperty::SlotType& func);
    std::shared_ptr<Gtk::TreeView> get_view();

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Container Widgets
    std::shared_ptr<Gtk::TreeView> s_view;
    std::unique_ptr<Gtk::ScrolledWindow> s_win;
    std::unique_ptr<Gtk::Box> s_box;

    // Widgets related to searchbar
    std::unique_ptr<Gtk::SearchEntry> s_search;
    std::unique_ptr<Gtk::CheckButton> s_use_regex;
    std::unique_ptr<Gtk::CheckButton> s_match_case;
    std::unique_ptr<Gtk::CheckButton> s_whole_word;
    std::unique_ptr<Gtk::Label>       s_found_label;


    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);    
    static Json::Value parse_JSON(const std::string& raw_json);
    static bool filter(const std::string& str, const std::string& rule, const bool& use_regex, const bool& match_case, const bool& whole_word);
};

#endif // TABS_STATUS_H
