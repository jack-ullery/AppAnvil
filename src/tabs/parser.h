#ifndef TABS_PARSER_H
#define TABS_PARSER_H

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Parser {
  public:
    static std::string get_perms(const std::string& filename);
  
  protected:
    static std::string ltrim(const std::string &s);
    static std::string handle_path(std::istringstream *is);
    
};

#endif // TABS_PARSER_H
