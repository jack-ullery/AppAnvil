#ifndef TABS_VIEW_APPARMOR_NOT_INSTALLED_H
#define TABS_VIEW_APPARMOR_NOT_INSTALLED_H

#include <gtkmm/messagedialog.h>

class AppArmorNotInstalled
{
public:
  /**
   * @brief Builds and shows the "AppArmor Not Installed" error message.
   */
  static std::unique_ptr<Gtk::MessageDialog> get_dialog();
};

#endif // TABS_VIEW_APPARMOR_NOT_INSTALLED_H