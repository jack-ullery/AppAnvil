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
  std::string placeholder;
  std::initializer_list<std::pair<std::string, std::string>> combobox_options;

  ColumnHeader(std::string _name, ColumnType _type)
    : type{ _type },
      name{ _name },
      placeholder()
  {
  }

  explicit ColumnHeader(std::string _name)
    : type{ STRING },
      name{ _name },
      placeholder()
  {
  }

  explicit ColumnHeader(std::string _name, std::string _placeholder, std::initializer_list<std::pair<std::string, std::string>> _combobox_options)
    : type{ COMBO_BOX },
      name{ _name },
      placeholder{ _placeholder },
      combobox_options{ _combobox_options }
  {
  }
};

#endif // TABS_COLUMN_HEADER_H