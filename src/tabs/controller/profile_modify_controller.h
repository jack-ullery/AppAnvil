#ifndef TABS_CONTROLLER_PROFILE_MODIFY_CONTROLLER_H
#define TABS_CONTROLLER_PROFILE_MODIFY_CONTROLLER_H

#include "../column_header.h"
#include "../model/status_column_record.h"
#include "../view/profile_modify.h"

#include <libappanvil/apparmor_parser.hh>
#include <memory>
#include <vector>

class ProfileModifyController
{
public:
  ProfileModifyController(std::shared_ptr<AppArmor::Parser> &parser, std::shared_ptr<AppArmor::Profile> &profile);

  std::shared_ptr<ProfileModify> get_profile_modify();

protected:
  void intialize_abstractions(std::shared_ptr<AppArmor::Profile> profile);
  void intialize_file_rules(std::shared_ptr<AppArmor::Profile> profile);

private:
  const std::vector<ColumnHeader> abstraction_col_names{
    ColumnHeader("Abstraction", ColumnHeader::ColumnType::STRING)
  };
  
  const std::vector<ColumnHeader> file_rule_col_names{ 
    ColumnHeader("Path", ColumnHeader::ColumnType::STRING),
    ColumnHeader("Filemode", ColumnHeader::ColumnType::STRING),
  };

  std::shared_ptr<ProfileModify> modify;
  std::shared_ptr<StatusColumnRecord> abstraction_record;
  std::shared_ptr<StatusColumnRecord> file_rule_record;
};

#endif // TABS_CONTROLLER_PROFILE_MODIFY_CONTROLLER_H
