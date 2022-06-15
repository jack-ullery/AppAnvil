#ifndef TABS_COLUMN_HEADER_H
#define TABS_COLUMN_HEADER_H

#include <string>

struct ColumnHeader {
  enum ColumnType 
  { 
    STRING, 
    INT 
  };

  ColumnType type;
  std::string name;

  ColumnHeader(std::string _name, ColumnType _type) 
    : type{_type}, 
      name{std::move(_name)} 
  { }

  explicit ColumnHeader(std::string _name) 
    : type{STRING}, 
      name{std::move(_name)}
  { }
};

#endif // TABS_COLUMN_HEADER_H