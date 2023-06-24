#ifndef TABS_COLUMN_HEADER_H
#define TABS_COLUMN_HEADER_H

#include <gtkmm/treemodel.h>
#include <string>

struct ColumnHeader
{
  enum ColumnType
  {
    STRING,
    INT,
    BOOLEAN,
    PROFILE_ENTRY,
    PROCESS_ENTRY,
    LOG_ENTRY,
    FILE_RULE_POINTER,
    COMBO_BOX,
  };

  ColumnType type;
  std::string name;
  std::initializer_list<std::pair<std::string, std::string>> combobox_options;

  ColumnHeader(std::string _name, ColumnType _type)
    : type{ _type },
      name{ std::move(_name) }
  {
  }

  explicit ColumnHeader(std::string _name)
    : type{ STRING },
      name{ std::move(_name) }
  {
  }

  explicit ColumnHeader(std::string _name, std::initializer_list<std::pair<std::string, std::string>> _combobox_options)
    : type{ COMBO_BOX },
      name{ std::move(_name) },
      combobox_options{ std::move(_combobox_options) }
  {
  }
};

#endif // TABS_COLUMN_HEADER_H