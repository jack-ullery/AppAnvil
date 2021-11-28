#include "parser.h"


const std::string WHITESPACE = " \n\r\t\f\v";

// these are all the flags which transition the resource to a subprofile
// we want to exclude these if we're just listing path + flags as they would be duplicates
std::array<std::string, 6> a{"Cx", "cx", "Cix", "cix", "CUx", "cux"};

// removes leading whitespace as we're reading a formatted source file
std::string Parser::ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

// parses the path and flags out of the passed string stream and excludes any entries
// that have one of the six transition-to-subprofile flags (in array 'a')
std::string Parser::handle_path(std::istringstream * is) {
	std::string path;
	std::string flags;
	std::getline(*is, path, ' ');
	std::getline(*is, flags, ',');
	flags = Parser::ltrim(flags);
	auto it = std::find_if(begin(a), end(a),
				[&](const std::string& s)
				{return flags.find(s) != std::string::npos; });
	if (it != end(a))
		return "";
	return "path: " + path + "\tmode: " + flags + '\n';
}

std::string Parser::get_perms(std::string filename) {
	std::ifstream fp(filename);
	if (!fp) {
		std::cerr << "cannot open profile for parsing\n";
		return "ERROR";
  	}

	// strings for holding tempory file input tokens
	std::string line;
	std::string token;
	std::string path;
	std::string flags;

	// strings containing the rules/permissions we need to eventually return to be parsed my caller
	std::string allow_str;
	std::string deny_str;
	std::string audit_str;
	std::string owner_str;

	// move file pointer to first '{' character
	std::getline(fp, line, '{'); 

	// scan every line into `line` string and check if it's a path to a resource/program with 
	// access mode flags and if it is then append it to the return string
	while (std::getline(fp, line, '\n')) {
		// remove leading whitespace such as indentation
		line = Parser::ltrim(line);

		if (line.size() <= 0) continue;
		if (line.at(0) == '#') continue;
		if (line.at(0) == '}') {
			// we hit a closing brace, no paths can exist between these braces so skip to the next block
			// WARNING: this assumes that every closing brace is on it's own new line (seems to be the case so far)
			std::getline(fp, line, '{');
			continue;
		}
		if (line.at(0) == '/') {
			// we have our path to the resource/program with no qualifier (i.e. default 'allow')
			// append to the string holding all rules with the allow qualifier
			std::istringstream is(line); // so we can use getline as a tokenizer
			allow_str += Parser::handle_path(&is);
			// left in a seperate if statement for any additions that may need to be made later
			// that would differentiate these from domain sockets that start with @	
		} else if (line.at(0) == '@') {
			// we have a UNIX domain socket which is also a path with no qualifier (i.e. default 'allow') 
			// append to the string holding all rules with the allow qualifier
			std::istringstream is(line); // so we can use getline as a tokenizer
			allow_str += Parser::handle_path(&is);
			// left in a seperate if statement for any additions that may need to be made later
			// that would differentiate these from regular paths that start with /
		} else {
			// check if the line starts with any of the 4 rule qualifiers (allow, deny, audit, owner)
			// if it does, append it to the corresponding string
			std::istringstream is(line); // so we can use getline as a tokenizer
			std::getline(is, token, ' ');
			if (token == "deny") {
				deny_str += Parser::handle_path(&is);
			} else if (token == "audit") {
				audit_str += Parser::handle_path(&is);
			} else if (token == "owner") {
				owner_str += Parser::handle_path(&is);
			} else if (token == "allow") {
				// assigned by default but should still check just in case someone explicitly declared it
				allow_str += Parser::handle_path(&is);
			}
		}
	}

	// temporary way of grouping for test output, return type can be changed to account for these categories
	std::cout << "\n\t\t\t\t\t --- Allow --- \n" + allow_str;
	std::cout << "\n\t\t\t\t\t --- Deny  --- \n" + deny_str;
	std::cout << "\n\t\t\t\t\t --- Audit --- \n" + audit_str;
	std::cout << "\n\t\t\t\t\t --- Owner --- \n" + owner_str;

	return allow_str + deny_str + audit_str + owner_str;
}
