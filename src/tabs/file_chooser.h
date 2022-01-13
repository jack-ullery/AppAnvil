#ifndef TABS_FILECHOOSER_H
#define TABS_FILECHOOSER_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/grid.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/filechooserdialog.h>
#include <memory>
#include <string>
#include <vector>

class FileChooser : public Gtk::ScrolledWindow
{
  public:
    FileChooser();
    //void add_data_to_record(std::string data);
    //void refresh();

  
  protected:

    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Widgets on the tab   
    std::unique_ptr<Gtk::Box> l_box;
    std::unique_ptr<Gtk::Label> l_upper_text;
    std::unique_ptr<Gtk::Grid> l_mid_grid;
    std::unique_ptr<Gtk::Button> l_filechooser_button;
    std::unique_ptr<Gtk::Label> l_filechooser_label;
    std::unique_ptr<Gtk::Label> l_lower_text;
    std::unique_ptr<Gtk::Grid> l_lower_grid;
    std::unique_ptr<Gtk::ComboBoxText> l_combo_profile_status_chooser;
    std::unique_ptr<Gtk::Button> l_confirm_button;

    // Signal handler
    void on_fc_button_clicked();
    void handle_f_diag(int response_id);
    void on_confirm_clicked();

    //Dialog box
    std::unique_ptr<Gtk::FileChooserDialog> f_diag;

    //Function called by signal handler
    void loadp(std::string fullFileName, std::string& feedBack);

  private:

    const std::vector<std::string> col_names{"Profile", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;

    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);
};

#endif // TABS_PROFILES_H
