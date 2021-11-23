#ifndef TABS_PARSER_H
#define TABS_PARSER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>

class Parser {
  public:
    static std::string get_perms(std::string filename);
  
  protected:
    static std::string ltrim(const std::string &s);
    static std::string handle_path(std::istringstream * is);
    
  private:
    Parser() {} // static class, no need to ever create an instance
    
};

#endif // TABS_PARSER_H
