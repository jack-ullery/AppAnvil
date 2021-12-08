#include "jsoncpp/json/json.h"
#include "file_chooser.h"
#include "../threads/command_caller.h"

#include <iostream>
#include <string>
#include <vector>


template <typename T_Widget>
std::unique_ptr<T_Widget> FileChooser::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}

FileChooser::FileChooser()
: builder{Gtk::Builder::create_from_file("./resources/load_profile.glade")},
  l_box{FileChooser::get_widget<Gtk::Box>("l_box", builder)},
  l_upper_text{FileChooser::get_widget<Gtk::Label>("l_upper_text", builder)},
  l_mid_grid{FileChooser::get_widget<Gtk::Grid>("l_mid_grid", builder)},
  l_filechooser_button{FileChooser::get_widget<Gtk::Button>("l_filechooser_button", builder)},
  l_filechooser_label{FileChooser::get_widget<Gtk::Label>("l_filechooser_label", builder)},
  l_lower_text{FileChooser::get_widget<Gtk::Label>("l_lower_text", builder)},
  l_lower_grid{FileChooser::get_widget<Gtk::Grid>("l_lower_grid", builder)},
  l_combo_profile_status_chooser{FileChooser::get_widget<Gtk::ComboBoxText>("l_combo_profile_status_chooser", builder)},
  l_confirm_button{FileChooser::get_widget<Gtk::Button>("l_confirm_button", builder)}
  //f_chooser{FileChooser::get_widget<Gtk::FileChooserWidget>("f_chooser", builder)}
{
  
  
  l_filechooser_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_fc_button_clicked));
  l_confirm_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_confirm_clicked));
  l_box->set_hexpand();
  l_box->set_vexpand();
  this->add(*l_box);
}



void FileChooser::on_fc_button_clicked(){

  // std::string feedback;
  // std::cout << f_chooser->get_filename() << std::endl;
  // CommandCaller::load_profile(f_chooser->get_filename(), "", feedback);
  // std::cout << feedback << std::endl;
  std::cout<< "FC" << std::endl;

  // auto dialog = new Gtk::FileChooserDialog("Please choose a file", Gtk::FileChooser::Action::OPEN);
  // dialog->set_transient_for(*this);
  // dialog->set_modal(true);
  // dialog->signal_response().connect(sigc::bind(
  // sigc::mem_fun(*this, &ExampleWindow::on_file_dialog_response), dialog));

  // //Add response buttons to the dialog:
  // dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  // dialog->add_button("_Open", Gtk::ResponseType::OK);

  // //Add filters, so that only certain file types can be selected:

  // auto filter_text = Gtk::FileFilter::create();
  // filter_text->set_name("Text files");
  // filter_text->add_mime_type("text/plain");
  // dialog->add_filter(filter_text);

  // auto filter_cpp = Gtk::FileFilter::create();
  // filter_cpp->set_name("C/C++ files");
  // filter_cpp->add_mime_type("text/x-c");
  // filter_cpp->add_mime_type("text/x-c++");
  // filter_cpp->add_mime_type("text/x-c-header");
  // dialog->add_filter(filter_cpp);

  // auto filter_any = Gtk::FileFilter::create();
  // filter_any->set_name("Any files");
  // filter_any->add_pattern("*");
  // dialog->add_filter(filter_any);

  // //Show the dialog and wait for a user response:
  // dialog->show();

  // Associate the f_diag pointer with a new Dialog
  f_diag.reset(new Gtk::FileChooserDialog("Choose a profile", Gtk::FILE_CHOOSER_ACTION_OPEN));
  // Close dialog when the OK button is pressed.

  f_diag->add_button("Confirm", Gtk::ResponseType::RESPONSE_OK);
  f_diag->signal_response().connect(sigc::mem_fun(*this, &FileChooser::handle_f_diag));
  f_diag->show();
  
}

void FileChooser::handle_f_diag(int response_id){
  auto filename = f_diag->get_file()->get_path();
  l_filechooser_label->set_text(filename);
  f_diag->hide();
}

void FileChooser::on_confirm_clicked(){
  auto choice = l_combo_profile_status_chooser->get_active_text();
  auto filename = l_filechooser_label->get_text();

  std::string short_filename = CommandCaller::load_profile(filename);
  if(choice == "Complain"){

    std::cout<< "C" << std::endl;
    CommandCaller::set_status(short_filename, "aa-complain");

  } else if(choice == "Enforce"){

    std::cout<< "E" << std::endl;
    CommandCaller::set_status(short_filename, "aa-enforce");


  } else if(choice == "Disable"){

    std::cout<< "D" << std::endl;
    CommandCaller::set_status(short_filename, "aa-disable");

  } else {

    std::cout<< "Nope" << std::endl;

  }

}


