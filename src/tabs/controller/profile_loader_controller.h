#ifndef TABS_CONTROLLER_PROFILELOADER_CONTROLLER_H
#define TABS_CONTROLLER_PROFILELOADER_CONTROLLER_H

#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <memory>
#include <string>
#include <vector>

template<class ProfileLoaderTab>
class ProfileLoaderController : public Gtk::ScrolledWindow
{
  public:
    ProfileLoaderController();
    void clearLabel();

    // Returns the Tab that this controller communicates with
    std::shared_ptr<ProfileLoaderTab> get_tab();

  protected:
    // Signal handler
    void on_confirm_clicked();

  private:
      std::shared_ptr<ProfileLoaderTab> fc;
};

#endif // TABS_CONTROLLER_PROFILELOADER_CONTROLLER_H
