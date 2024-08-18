#include "apparmor_not_installed.h"
#include "common.h"

std::unique_ptr<Gtk::MessageDialog> AppArmorNotInstalled::get_dialog()
{
  auto builder = Gtk::Builder::create_from_resource("/apparmor_not_installed.glade");
  auto dialog = Common::get_widget<Gtk::MessageDialog>("apparmor_not_installed", builder);

  return dialog;
}
