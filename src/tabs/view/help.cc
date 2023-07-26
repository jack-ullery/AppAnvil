#include "common.h"
#include "help.h"

Help::Help()
  : builder{ Gtk::Builder::create_from_resource("/resources/help.glade") },
    help_profile{ Common::get_widget<Gtk::Popover>("help_profile", builder) },
    help_profile_modify{ Common::get_widget<Gtk::Popover>("help_profile_modify", builder) },
    help_logs{ Common::get_widget<Gtk::Popover>("help_logs", builder) }
{
  // Set the image for this button
  this->set_image_from_icon_name("dialog-question");

  // Make popups point to this ToggleButton
  help_profile->set_relative_to(*this);
  help_profile_modify->set_relative_to(*this);
  help_logs->set_relative_to(*this);

  // Ensure popups are not modal
  // That other inputs on the screen can be pressed when they are displayed
  help_profile->set_modal(false);
  help_profile_modify->set_modal(false);
  help_logs->set_modal(false);

  auto help_toggle_fun = sigc::mem_fun(*this, &Help::on_help_toggle);
  this->signal_toggled().connect(help_toggle_fun, true);
}

void Help::on_help_toggle()
{
  bool active = this->get_active();

  if(active) {
    help_profile->show();
  } else {
    help_profile->hide();
  }
}