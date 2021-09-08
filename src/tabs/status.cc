#include "jsoncpp/json/json.h"
#include "status.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <sstream>
#include <string>

Json::Value Status::parse_JSON(const std::string& raw_json){
  std::stringstream stream;
  stream << raw_json;

  Json::Value root;
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  
  if (!parseFromStream(builder, stream, &root, &errs)) {
    std::cout << errs << std::endl;
  }

  return root;
}

std::string Status::get_status_str(){
  std::vector<std::string> args = {"sudo", "aa-status","--json"};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = "{\"processes\": {}, \"profiles\": {}";
  }

  return child_output;
}

Json::Value Status::get_status_JSON(){
  return parse_JSON(get_status_str());
}