#include "profile_modify_controller.h"

#include <iostream>
#include <memory>

std::shared_ptr<ProfileModify> ProfileModifyController::get_profile_modify()
{
  return modify;  
}

void ProfileModifyController::intialize_abstractions()
{
  abstraction_record->clear();
  auto abstractions = this->profile->getAbstractions();

  for(auto &abstraction : abstractions) {
    // Find the last slash, and use that information to extract the filename from the abstraction
    std::string abstraction_str = abstraction.getPath();
    auto pos = abstraction_str.find_last_of('/');
    if(pos == std::string::npos) {
      pos = 0;
    }
    else {
      pos++;
    }

    std::string trimmed = abstraction_str.substr(pos);

    auto row = abstraction_record->new_row();
    row->set_value(0, trimmed);
  }
}

void ProfileModifyController::intialize_file_rules()
{
  file_rule_record->clear();
  auto rules = this->profile->getFileRules();

  for(AppArmor::Tree::FileRule &rule : rules) {
    auto shared_rule = std::make_shared<AppArmor::Tree::FileRule>(rule);
    const std::string filename = rule.getFilename();
    const auto filemode = rule.getFilemode();

    auto row = file_rule_record->new_row();
    row->set_value(FILE_RULE_POS::Data,  shared_rule);
    row->set_value(FILE_RULE_POS::Path,  filename);
    row->set_value(FILE_RULE_POS::Read,  filemode.getRead());
    row->set_value(FILE_RULE_POS::Write, filemode.getWrite());
    row->set_value(FILE_RULE_POS::Link,  filemode.getLink());
    row->set_value(FILE_RULE_POS::Lock,  filemode.getLock());
    row->set_value(FILE_RULE_POS::Exec, !filemode.getExecuteMode().empty());    

    auto toggle_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_file_rule_toggle);
    file_rule_record->set_toggle_func(toggle_fun);
  }
}

void ProfileModifyController::update_all_tables()
{
  intialize_abstractions();
  intialize_file_rules();
}

void ProfileModifyController::handle_file_rule_toggle(const Gtk::TreeModel::iterator &node)
{
  // Get the FileRule stored in the row
  std::shared_ptr<AppArmor::Tree::FileRule> rule;
  node->get_value(FILE_RULE_POS::Data, rule);
  
  if(rule != nullptr) {
    const std::string filename = rule->getFilename();
    const auto filemode = rule->getFilemode();

    bool read       = filemode.getRead();
    bool write      = filemode.getWrite();
    bool append     = filemode.getAppend();
    bool memory_map = filemode.getMemoryMap();
    bool link       = filemode.getLink();
    bool lock       = filemode.getLock();
    bool has_exec   = false;

    node->get_value(FILE_RULE_POS::Read,  read);
    node->get_value(FILE_RULE_POS::Write, write);
    node->get_value(FILE_RULE_POS::Link,  link);
    node->get_value(FILE_RULE_POS::Lock,  lock);
    node->get_value(FILE_RULE_POS::Exec,  has_exec);

    std::string exec_mode;
    if(has_exec) {
      exec_mode = filemode.getExecuteMode();
      if(exec_mode.empty()) {
        exec_mode = "ix";
      }
    }

    AppArmor::Tree::FileMode new_filemode(read, write, append, memory_map, link, lock, exec_mode);
    AppArmor::Tree::FileRule new_rule(0, -1, rule->getFilename(), new_filemode, rule->getExecTarget(), rule->getIsSubset());

    handle_edit_file_rule(*rule, new_rule);
  } else {
    std::cerr << "Error: Could not locate AppArmor::FileRule for selected row" << std::endl;
  }
}

void ProfileModifyController::handle_edit_file_rule(AppArmor::Tree::FileRule &old_rule, const AppArmor::Tree::FileRule &new_rule)
{
  std::cout << "New Rule: " << std::string(new_rule) << std::endl;
  parser->editRule(*profile, old_rule, new_rule, profile_stream);

  // Find the new parsed profile from the list of profiles
  for(auto new_profile : parser->getProfileList()) {
    if(new_profile.name() == profile->name()) {
      profile = std::make_shared<AppArmor::Profile>(new_profile);
    }
  }

  // Update all the rows and tables
  update_all_tables();
}

ProfileModifyController::ProfileModifyController(std::shared_ptr<AppArmor::Parser> parser,
                                                 std::shared_ptr<AppArmor::Profile> profile)
  : parser{ parser },
    profile{ profile },
    modify{ std::make_shared<ProfileModify>(parser, profile) },
    abstraction_record{ StatusColumnRecord::create(modify->get_abstraction_view(), abstraction_col_names)},
    file_rule_record{ StatusColumnRecord::create(modify->get_file_rule_view(), file_rule_col_names)}
{
  update_all_tables();
}