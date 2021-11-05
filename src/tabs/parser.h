#ifndef TABS_PARSER_H
#define TABS_PARSER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class Parser {
  public:
    static std::string get_perms(std::string filename);
    static std::string ltrim(const std::string &s);
    
  private:
    Parser() {} // static class, no need to ever create an instance
};

#endif // TABS_PARSER_H
