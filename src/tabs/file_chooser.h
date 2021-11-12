#ifndef TABS_FILECHOOSER_H
#define TABS_FILECHOOSER_H

#include "status.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/enums.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/filechooserwidget.h>
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
    std::unique_ptr<Gtk::Button> f_button;
    std::unique_ptr<Gtk::Box> f_box;
    std::unique_ptr<Gtk::FileChooserWidget> f_chooser;

    // Signal handler
    void on_button_clicked();

    //Function called by signal handler
    void loadp(std::string fullFileName, std::string& feedBack);

  private:

    const std::vector<std::string> col_names{"Profile", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;

    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);
};

#endif // TABS_PROFILES_H
