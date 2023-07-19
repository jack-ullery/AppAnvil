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

  for (auto &abstraction : abstractions) {
    // Find the last slash, and use that information to extract the filename from the abstraction
    std::string abstraction_str = abstraction.getPath();
    auto pos                    = abstraction_str.find_last_of('/');
    if (pos == std::string::npos) {
      pos = 0;
    } else {
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

  for (AppArmor::Tree::FileRule &rule : rules) {
    auto shared_rule           = std::make_shared<AppArmor::Tree::FileRule>(rule);
    const std::string filename = rule.getFilename();
    const auto filemode        = rule.getFilemode();
    const auto prefix = rule.getPrefix();

    auto row = file_rule_record->new_row();
    row->set_value(FILE_RULE_POS::Data, shared_rule);
    row->set_value(FILE_RULE_POS::Path, filename);
    row->set_value(FILE_RULE_POS::Read, filemode.getRead());
    row->set_value(FILE_RULE_POS::Write, filemode.getWrite());
    row->set_value(FILE_RULE_POS::Link, filemode.getLink());
    row->set_value(FILE_RULE_POS::Lock, filemode.getLock());
    row->set_value(FILE_RULE_POS::Exec, filemode.getExecuteMode());
    row->set_value(FILE_RULE_POS::Advanced, prefix.operator std::string());

    auto change_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_file_rule_changed);
    file_rule_record->set_change_func(change_fun);
  }
}

void ProfileModifyController::update_all_tables()
{
  intialize_abstractions();
  intialize_file_rules();
}

void ProfileModifyController::handle_profile_changed()
{
  // Find the new parsed profile from the list of profiles
  for (auto new_profile : parser->getProfileList()) {
    if (new_profile.name() == profile->name()) {
      profile = std::make_shared<AppArmor::Profile>(new_profile);
    }
  }

  // Update all the rows and tables
  update_all_tables();
}

void ProfileModifyController::handle_file_rule_changed(const std::string &path)
{
  auto row = file_rule_record->get_iter(path);

  // Get the FileRule stored in the row
  std::shared_ptr<AppArmor::Tree::FileRule> rule;
  row->get_value(FILE_RULE_POS::Data, rule);

  if (rule != nullptr) {
    const std::string filename = rule->getFilename();
    const auto filemode        = rule->getFilemode();

    bool read       = filemode.getRead();
    bool write      = filemode.getWrite();
    bool append     = filemode.getAppend();
    bool memory_map = filemode.getMemoryMap();
    bool link       = filemode.getLink();
    bool lock       = filemode.getLock();
    std::string exec_mode;

    row->get_value(FILE_RULE_POS::Read, read);
    row->get_value(FILE_RULE_POS::Write, write);
    row->get_value(FILE_RULE_POS::Link, link);
    row->get_value(FILE_RULE_POS::Lock, lock);
    row->get_value(FILE_RULE_POS::Exec, exec_mode);

    AppArmor::Tree::FileMode new_filemode(read, write, append, memory_map, link, lock, exec_mode);

    if (new_filemode.empty()) {
      handle_remove_rule(*rule);
    } else {
      AppArmor::Tree::FileRule new_rule(0, -1, rule->getFilename(), new_filemode, rule->getExecTarget(), rule->getIsSubset());
      handle_edit_rule(*rule, new_rule);
    }
  } else {
    std::cerr << "Error: Could not locate AppArmor::FileRule for selected row" << std::endl;
  }
}

void ProfileModifyController::handle_edit_rule(AppArmor::Tree::FileRule &old_rule, const AppArmor::Tree::FileRule &new_rule)
{
  std::cout << "Edited Rule: " << std::string(new_rule) << std::endl;
  parser->editRule(*profile, old_rule, new_rule, profile_stream);
  handle_profile_changed();
}

void ProfileModifyController::handle_remove_rule(AppArmor::Tree::FileRule &old_rule)
{
  std::cout << "Removed Rule: " << old_rule.getFilename() << std::endl;
  parser->removeRule(*profile, old_rule, profile_stream);
  handle_profile_changed();
}

ProfileModifyController::ProfileModifyController(std::shared_ptr<AppArmor::Parser> parser, std::shared_ptr<AppArmor::Profile> profile)
  : parser{ parser },
    profile{ profile },
    modify{ std::make_shared<ProfileModify>(parser, profile) },
    abstraction_record{ StatusColumnRecord::create(modify->get_abstraction_view(), abstraction_col_names) },
    file_rule_record{ StatusColumnRecord::create(modify->get_file_rule_view(), file_rule_col_names) }
{
  auto change_fun = sigc::mem_fun(*this, &ProfileModifyController::handle_file_rule_changed);
  file_rule_record->set_change_func(change_fun);

  update_all_tables();
}
