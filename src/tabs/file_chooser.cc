#include "jsoncpp/json/json.h"
#include "file_chooser.h"

#include <iostream>
#include <string>
#include <vector>


template <typename T_Widget>
std::unique_ptr<T_Widget> FileChooser::get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder){
  T_Widget *raw_addr = nullptr;
  builder->get_widget<T_Widget>(name, raw_addr);
  return std::unique_ptr<T_Widget>(raw_addr);
}
// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
// void FileChooser::add_data_to_record(std::string data){
//   Json::Value root = Status::parse_JSON(data);
//   Json::Value filechooser = root["filechooser"];

//   int num_found = 0;

//   col_record->clear();
//   for(auto prof = filechooser.begin(); prof != filechooser.end(); prof++){
//     std::string key = prof.key().asString();
//     auto row = col_record->new_row();
//     col_record->set_row_data(row, 0, key);
//     col_record->set_row_data(row, 1, filechooser.get(key, UNKNOWN_STATUS).asString());
//     num_found++;
//   }

//   Status::set_status_label_text(" " + std::to_string(num_found) + " matching filechooser");
// }

// void FileChooser::refresh(){
//   std::cout << "Refresh called!" << std::endl;
//   auto filter_fun = sigc::mem_fun(*this, &FileChooser::filter);
//   col_record->filter_rows(filter_fun);
// }

FileChooser::FileChooser()
: builder{Gtk::Builder::create_from_file("./resources/file_chooser.glade")},
  f_button{FileChooser::get_widget<Gtk::Button>("f_button", builder)},
  f_box{FileChooser::get_widget<Gtk::Box>("f_box", builder)},
  f_chooser{FileChooser::get_widget<Gtk::FileChooserWidget>("f_chooser", builder)}
{
  
  // auto refresh_func = sigc::mem_fun(*this, &FileChooser::refresh);
  //auto apply_func = sigc::mem_fun(*this, &FileChooser::on_apply_button_pressed);
  // Status::set_refresh_signal_handler(refresh_func);
  //Status::set_apply_signal_handler(apply_func);

  // f_box->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
  // f_box->set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
  f_button->signal_clicked().connect(sigc::mem_fun(*this, &FileChooser::on_button_clicked));
  f_box->set_hexpand();
  f_box->set_vexpand();

  this->add(*f_box);
}

void FileChooser::on_button_clicked(){

  std::string feedback;
  std::cout << f_chooser->get_filename() << std::endl;
  loadp(f_chooser->get_filename(), feedback);
  std::cout << feedback << std::endl;
}

void FileChooser::loadp(std::string fullFileName, std::string& feedBack){

	const int N = 300;

	std::string fileName;	//name without path
	fileName = fullFileName.substr(fullFileName.find_last_of("/") + 1);

	std::string scmd = "echo \"123\" | sudo mv " + fullFileName + " /etc/apparmor.d";
	//const char* cmd1 = scmd.c_str();
	//system(cmd1);
	system(scmd.c_str());

	char line[N];
	FILE *fp;

	scmd = "echo \"123\" | sudo apparmor_parser -r /etc/apparmor.d/" + fileName + " 2>&1";
	//const char* cmd2 = scmd.c_str();
	//fp = popen(cmd2, "r");
	fp = popen(scmd.c_str(), "r");

	//error message
	std::string fb = "";
	std::string sl;
	while(fgets(line, sizeof(line)-1, fp) !=NULL){
		sl = line;
		fb += sl;
	}


	feedBack = fb;


	pclose(fp);

}