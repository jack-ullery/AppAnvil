#include "help.h"
#include "common.h"

Help::Help(Help::Type help_type)
  : builder{ Gtk::Builder::create_from_resource("/modal/help.glade") },
    help_profile{ Common::get_widget<Gtk::Popover>("help_profile", builder) },
    help_process{ Common::get_widget<Gtk::Popover>("help_process", builder) },
    help_logs{ Common::get_widget<Gtk::Popover>("help_logs", builder) },
    help_type{ help_type }
{
  // Set the image for this button
  this->set_image_from_icon_name("dialog-question");

  // Make popups point to this ToggleButton
  help_profile->set_relative_to(*this);
  help_process->set_relative_to(*this);
  help_logs->set_relative_to(*this);

  // Ensure popups are not modal
  // That other inputs on the screen can be pressed when they are displayed
  help_profile->set_modal(false);
  help_process->set_modal(false);
  help_logs->set_modal(false);

  auto help_toggle_fun = sigc::mem_fun(*this, &Help::on_help_toggle);
  this->signal_toggled().connect(help_toggle_fun, true);
}

void Help::set_help_type(Help::Type help_type)
{
  this->help_type = help_type;

  // If the help button is toggled, then update the popover
  on_help_toggle();
}

void Help::on_help_toggle()
{
  bool active = this->get_active();

  help_profile->hide();
  help_process->hide();
  help_logs->hide();

  if (active) {
    switch (help_type) {
      case Help::Type::PROFILE:
        help_profile->show();
        break;

      case Help::Type::PROCESS:
        help_process->show();
        break;

      case Help::Type::LOGS:
        help_logs->show();
        break;
    };
  }
}