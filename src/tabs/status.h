#ifndef TABS_STATUS_H
#define TABS_STATUS_H

#include "jsoncpp/json/json.h"
#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/spinner.h>
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

    /**
     * @brief Parse the output of `aa-status --json`
     * 
     * @details
     * Parses the output of `pkexec aa-status --json` to get a list of profiles and processes confined by apparmor. 
     * 
     * @returns Json::Value the output of aa-status as parsed by jsoncpp
     */
    static Json::Value get_status_JSON();

    /**
     * @brief Return the output of `aa-status --json`
     * 
     * @details
     * Returns the output of `pkexec aa-status --json` to get a list of profiles and processes confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-status
     */
    static std::string get_logs_str();

    /**
     * @brief Return the output of `aa-unconfined`
     * 
     * @details
     * Returns the output of `pkexec aa-unconfined` to get a list of processes not confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-unconfined
     */
    static std::string get_unconfined();

  protected:
    /**
     * @brief Decide whether a string should be added to the table
     * 
     * @details
     * Takes a string as input and decides whether it should be added the table.
     * Uses the text in the searchbar and the state of the checkboxes to determine
     * whether the string can be added.
     *  
     * @returns `true` if the string should be added, `false` if it should not
     */
    bool filter(const std::string& str);

    /**
     * @brief Change the text in the label directly above the searchbar.
     */
    void set_status_label_text(const std::string& str);

    /**
     * @brief Set the method to be called every time the search area updated.
     * 
     * @details
     * Sets the signal handler method to be called every time the search area is updated. This
     * happens when the user clicks a checkbutton, or types into the searchbar.
     * 
     * The signal handler should have the following protoype: `void method_name()`
     */
    void set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType& func);

    /**
     * @brief Set the method to be called every time the apply button is clicked.
     * 
     * @details
     * Sets the signal handler method to be called every time the apply button is clicked. This
     * happens when the user clicks the 'Apply' button above the search bar.
     * 
     * The signal handler should have the following protoype: `void method_name()`
     */
    void set_apply_signal_handler(const Glib::SignalProxyProperty::SlotType& func);

    /**
     * @brief Return the TreeView associated with this class.
     * @returns The TreeView data member used by this class.
     */
    std::shared_ptr<Gtk::TreeView> get_view();

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Container Widgets
    std::shared_ptr<Gtk::TreeView> s_view;
    std::shared_ptr<Gtk::TreeSelection> s_row;
    std::unique_ptr<Gtk::ScrolledWindow> s_win;
    std::unique_ptr<Gtk::Box> s_box;

    // Widgets related to searchbar
    std::unique_ptr<Gtk::SearchEntry> s_search;
    std::unique_ptr<Gtk::CheckButton> s_use_regex;
    std::unique_ptr<Gtk::CheckButton> s_match_case;
    std::unique_ptr<Gtk::CheckButton> s_whole_word;
    std::unique_ptr<Gtk::Label>       s_found_label;

    // Widgets related to changing profile status (above search)
    std::unique_ptr<Gtk::Button>      s_apply_button;
    std::unique_ptr<Gtk::Spinner>     s_spinner;
    std::unique_ptr<Gtk::Label>       s_apply_info_text;
    std::unique_ptr<Gtk::ComboBox>    s_status_selection;




    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);    
    static Json::Value parse_JSON(const std::string& raw_json);
    static bool filter(const std::string& str, const std::string& rule, const bool& use_regex, const bool& match_case, const bool& whole_word);
};

#endif // TABS_STATUS_H
