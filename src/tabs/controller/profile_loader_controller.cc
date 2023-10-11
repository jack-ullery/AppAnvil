#include "profile_loader_controller.h"
#include "../../threads/command_caller.h"
#include "../view/profile_loader.h"

// TODO(apparmor): Need to make asynchronous eventually, there should be no direct calls to CommandCaller
template<class ProfileLoaderTab>
void ProfileLoaderControllerImpl<ProfileLoaderTab>::on_confirm_clicked()
{
  // In production-code, this is a `Glib::RefPtr<Gio::File>`
  auto file = fc->get_file();

  // If the file is not null, then attempt to load it as a new profiile
  if (file) {
    auto filename              = file->get_path();
    std::string short_filename = CommandCaller::load_profile(filename);

    // TODO(apparmor): Add error handling, when the profile is not correctly formatted
    fc->set_label_text("Done");
  } else {
    fc->set_label_text("No profile selected!");
  }
}

template<class ProfileLoaderTab>
void ProfileLoaderControllerImpl<ProfileLoaderTab>::clearLabel()
{
  fc->set_label_text("");
}

template<class ProfileLoaderTab>
std::shared_ptr<ProfileLoaderTab> ProfileLoaderControllerImpl<ProfileLoaderTab>::get_tab()
{
  return fc;
}

template<class ProfileLoaderTab>
ProfileLoaderControllerImpl<ProfileLoaderTab>::ProfileLoaderControllerImpl()
  : fc{ new ProfileLoaderTab() }
{
  auto button_func = sigc::mem_fun(*this, &ProfileLoaderControllerImpl::on_confirm_clicked);
  fc->set_l_button_signal_handler(button_func);
}

// Used to avoid linker errors
template class ProfileLoaderControllerImpl<ProfileLoader>;
