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
  ProfileModifyController(std::shared_ptr<AppArmor::Parser> parser, std::shared_ptr<AppArmor::Profile> profile);

  std::shared_ptr<ProfileModify> get_profile_modify();

protected:
  void intialize_abstractions();
  void intialize_file_rules();
  void update_all_tables();

  void handle_file_rule_toggle(const Gtk::TreeModel::iterator &node);
  void handle_edit_file_rule(AppArmor::Tree::FileRule &old_rule, const AppArmor::Tree::FileRule &new_rule);

private:
  const std::vector<ColumnHeader> abstraction_col_names{
    ColumnHeader("Abstraction", ColumnHeader::ColumnType::STRING)
  };

  const std::vector<ColumnHeader> file_rule_col_names{
    ColumnHeader("Data", ColumnHeader::ColumnType::FILE_RULE_POINTER),
    ColumnHeader("File Path", ColumnHeader::ColumnType::STRING),
    ColumnHeader("Read", ColumnHeader::ColumnType::BOOLEAN),
    ColumnHeader("Write", ColumnHeader::ColumnType::BOOLEAN),
    ColumnHeader("Link", ColumnHeader::ColumnType::BOOLEAN),
    ColumnHeader("Lock", ColumnHeader::ColumnType::BOOLEAN),
    ColumnHeader("Execute", ColumnHeader::ColumnType::BOOLEAN),
    ColumnHeader("Advanced", ColumnHeader::ColumnType::STRING),
  };

  enum FILE_RULE_POS {
    Data  = 0,
    Path  = 1,
    Read  = 2,
    Write = 3,
    Link  = 4,
    Lock  = 5,
    Exec  = 6,
  };

  std::stringstream profile_stream;
  std::shared_ptr<AppArmor::Parser> parser;
  std::shared_ptr<AppArmor::Profile> profile;

  std::shared_ptr<ProfileModify> modify;
  std::shared_ptr<StatusColumnRecord> abstraction_record;
  std::shared_ptr<StatusColumnRecord> file_rule_record;
};

#endif // TABS_CONTROLLER_PROFILE_MODIFY_CONTROLLER_H
