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
    ABSTRACTION_RULE_POINTER,
    FILE_RULE_POINTER,
    COMBO_BOX,
  };

  ColumnType type;
  std::string name;
  std::string placeholder;
  std::initializer_list<std::pair<std::string, std::string>> combobox_options;

  ColumnHeader(const std::string &_name, const ColumnType &_type)
    : type{ _type },
      name{ _name }
  {
  }

  explicit ColumnHeader(const std::string &_name)
    : type{ STRING },
      name{ _name }
  {
  }

  explicit ColumnHeader(const std::string &_name,
                        const std::string &_placeholder,
                        std::initializer_list<std::pair<std::string, std::string>> _combobox_options)
    : type{ COMBO_BOX },
      name{ _name },
      placeholder{ _placeholder },
      combobox_options(_combobox_options)
  {
  }
};

#endif // TABS_COLUMN_HEADER_H