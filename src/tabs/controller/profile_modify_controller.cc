#include "profile_modify_controller.h"

#include <iostream>
#include <memory>

std::shared_ptr<ProfileModify> ProfileModifyController::get_profile_modify()
{
  return modify;  
}

void ProfileModifyController::intialize_abstractions(std::shared_ptr<AppArmor::Profile> profile)
{
  auto abstractions = profile->getAbstractions();

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

void ProfileModifyController::intialize_file_rules(std::shared_ptr<AppArmor::Profile> profile)
{
  auto rules = profile->getFileRules();

  for(AppArmor::Tree::FileRule &rule : rules) {
    const std::string filename = rule.getFilename();
    const auto filemode = rule.getFilemode();

    auto row = file_rule_record->new_row();
    row->set_value(0, filename);
    row->set_value(1, filemode.getRead());
    row->set_value(2, filemode.getWrite());
    row->set_value(3, filemode.getLink());
    row->set_value(4, filemode.getLock());
    row->set_value(5, !filemode.getExecuteMode().empty());
  }
}

ProfileModifyController::ProfileModifyController(std::shared_ptr<AppArmor::Parser> &parser,
                                                 std::shared_ptr<AppArmor::Profile> &profile)
  : modify{ std::make_shared<ProfileModify>(parser, profile) },
    abstraction_record{ StatusColumnRecord::create(modify->get_abstraction_view(), abstraction_col_names)},
    file_rule_record{ StatusColumnRecord::create(modify->get_file_rule_view(), file_rule_col_names)}
{
  intialize_abstractions(profile);
  intialize_file_rules(profile);
}
