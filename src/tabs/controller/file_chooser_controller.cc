#include "file_chooser_controller.h"
#include "../view/file_chooser.h"
#include "../../threads/command_caller.h"

// TODO: Need to make asynchronous eventually, there should be no direct calls to CommandCaller
template<class FileChooserTab>
void FileChooserController<FileChooserTab>::on_confirm_clicked()
{
  // In production-code, this is a `Glib::RefPtr<Gio::File>`
  auto file = fc->get_file();
  
  // If the file is not null, then attempt to load it as a new profiile
  if (file) {
  	auto filename = file->get_path(); 
	  std::string short_filename = CommandCaller::load_profile(filename);

    // TODO: Add error handling, when the profile is not correctly formatted

	  fc->set_label_text("Done");
	}
  else {
	  fc->set_label_text("No profile selected!");
  }
}

template<class FileChooserTab>
void FileChooserController<FileChooserTab>::clearLabel(){
  fc->set_label_text("");
}

template<class FileChooserTab>
std::shared_ptr<FileChooserTab> FileChooserController<FileChooserTab>::get_tab()
{
  return fc;
}

template<class FileChooserTab>
FileChooserController<FileChooserTab>::FileChooserController()
  : fc{new FileChooserTab()}
{
  auto button_func = sigc::mem_fun(*this, &FileChooserController::on_confirm_clicked);
  fc->set_l_button_signal_handler(button_func);
}

// Used to avoid linker errors
template class FileChooserController<FileChooser>;
