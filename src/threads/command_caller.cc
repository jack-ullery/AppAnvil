#include "command_caller.h"

#include <glibmm/spawn.h>
#include <iostream>
#include <vector>

///*
#include <cstdlib>
#include <cstdio>
//*/


std::string CommandCaller::get_status_str(){
  std::vector<std::string> args = {"pkexec", "aa-status","--json"};

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

//Status must be aa-complain, aa-enforce, or aa-disable
std::string CommandCaller::set_status(std::string profileName, std::string status){
  std::vector<std::string> args = {"pkexec", status, profileName};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;


  //Executed in commandline, copied from status.cc 
  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);


  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = child_error;
  }

  return child_output;
}

std::string CommandCaller::get_unconfined(){
  std::vector<std::string> args = {"pkexec", "aa-unconfined"};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = "";
  }

  return child_output;
}

std::string CommandCaller::get_logs_str(){
  std::vector<std::string> args = {"dmesg"};

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;
    child_output = "";
  }

  return child_output;
}


// Code modified from Pavl-o's fork
std::string CommandCaller::execute_change(const std::string& profile, const std::string& old_status, const std::string& new_status){
  std::string status_command;
  if (new_status == "enforce" && old_status != "enforce") {
    status_command = "aa-enforce";
  } else if (new_status == "complain" && old_status != "complain") {
    status_command = "aa-complain";
  } else if (new_status == "disable" && old_status != "disabled") {
    status_command = "aa-disable";
  } else if (new_status == old_status){
    return "'" + profile + "' already set to " + new_status + ".";
  } else {
    return "Error: Illegal arguments passed to CommandCaller::execute_change.";
  }

   // executed in commandline, copied from status.cc 

  std::string child_output;
  std::string child_error;
  int exit_status = 0;
  
  std::vector<std::string> args = {"pkexec", status_command, profile};
  Glib::spawn_sync("/usr/sbin/", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    return " Error changing the status of '" + profile + "': " + child_error;
  }

  return " Changed '" + profile + "' from " + old_status + " to " + new_status;
}

std::string CommandCaller::load_profile(std::string fullFileName){


  std::vector<std::string> args = {"pkexec", "mv", fullFileName, "/etc/apparmor.d"};
  //std::vector<std::string> args = {"echo", password, "|", "sudo", "-S", "mv", fullFileName, "/etc/apparmor.d"};
  // command to change the profile to the provided status

  std::string child_output;
  std::string child_error;
  int exit_status = 0;

  Glib::spawn_sync(".", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;

    return child_error;
  }


  std::string fileName; //name without path
  fileName = fullFileName.substr(fullFileName.find_last_of("/") + 1);

  args = {"pkexec", "apparmor_parser", "-r", "/etc/apparmor.d/" + fileName};
 // args = {"echo", password, "|", "sudo", "-S", "apparmor_parser", "-r", "/etc/apparmor.d/" + fileName};

  exit_status = 0;

  Glib::spawn_sync(".", args, Glib::SpawnFlags::SPAWN_DEFAULT, {}, &child_output, &child_error, &exit_status);

  if(exit_status != 0){
    std::cout << "Error calling '"<< args[0] <<"'. " << child_error << std::endl;

    return child_error;
  }

  //std::cout<<"load_profile ending\n";
  return fileName;

}
 
