#ifndef TABS_COLUMN_HEADER_H
#define TABS_COLUMN_HEADER_H

#include <string>

struct ColumnHeader {
  enum ColumnType { STRING, INT };

  ColumnType type;
  std::string name;

  ColumnHeader(std::string name, ColumnType type) : type{type}, name{std::move(name)} { }

  explicit ColumnHeader(std::string name) : type{STRING}, name{std::move(name)} { }
};

#endif // TABS_COLUMN_HEADER_H