#ifndef SRC_THREADS_COMMAND_CALLER
#define SRC_THREADS_COMMAND_CALLER

#include <apparmor_parser.hh>
#include <apparmor_profile.hh>
#include <map>
#include <string>
#include <vector>

#ifdef TESTS_ENABLED
#include <gtest/gtest.h>
#endif

/**
 * Calls commands on the terminal to be used by the rest of the program.
 * This is where AppAnvil actually interfaces with AppArmor.
 * Most of these functions are called on the second thread.
 **/
class CommandCaller
{
public:
  // Default constructor and destructor
  CommandCaller()          = default;
  virtual ~CommandCaller() = default;

  // Declare implicit copy and move constructors
  CommandCaller(const CommandCaller &)            = default;
  CommandCaller(CommandCaller &&)                 = delete;
  CommandCaller &operator=(const CommandCaller &) = default;
  CommandCaller &operator=(CommandCaller &&)      = delete;

  /**
   * @brief Return the output of `aa-status --json`
   *
   * @details
   * Returns the output of `pkexec aa-status --json` to get a list of profiles and processes confined by apparmor.
   *
   * @returns std::string the raw output of aa-status
   */
  static std::string get_status();

  /**
   * @brief Return the output of `aa-unconfined`
   *
   * @details
   * Returns the output of `pkexec aa-unconfined` to get a list of processes not confined by apparmor.
   *
   * @returns std::string the raw output of aa-unconfined
   */
  static std::string get_unconfined();

  /**
   * @brief Change the status of a profile
   *
   * @details
   * Change the status of a profile from `old_status` to `new_status` using `aa-enforce`, `aa-complain`, or `aa-disable`.
   * Valid names for variables `old_status` and `new_status` are: "enforce", "complain", "disable".
   * May eventually want to move from strings to enums.
   *
   * @returns std::string
   */
  static std::string execute_change(const std::string &profile, const std::string &old_status, const std::string &new_status);

  /*
    loads a profile given its file name, puts it into enforce mode
  */
  static std::string load_profile(const std::string &fullFileName);

  static std::string disable_profile(const std::string &profileName);

  // Returns true if the file at the path
  static bool file_exists(const std::string &location);

  // Attempt to locate the profile in possible locations
  static std::string locate_profile(const std::string &profile, const std::initializer_list<std::string> &possible_profile_locations = {"/etc/apparmor.d/", "/var/lib/snapd/apparmor/profiles/"});

  // Gets a vector of abstractions located at a path (default "/etc/apparmor.d")
  static std::vector<std::string> get_abstractions(const std::string &path = "/etc/apparmor.d/abstractions");

  // Returns a map (indexed by profile name) for each AppArmor::Profile found at certain directories
  // This function attempts to parse every file at specified locations to search for profiles
  static std::map<std::string, AppArmor::Profile> get_profiles(const std::initializer_list<std::string> &possible_profile_locations = {"/etc/apparmor.d/", "/var/lib/snapd/apparmor/profiles/"});

protected:
  struct results
  {
    int exit_status = 0;
    std::string output;
    std::string error;
  };

  // Used to call command-line commands from `/usr/sbin`
  virtual results call_command(const std::vector<std::string> &command);
  virtual std::string call_command(const std::vector<std::string> &command, const std::string &return_on_error);

  // Dependency Injection: For unit testing
  static std::string get_status(CommandCaller *caller);
  static std::string get_unconfined(CommandCaller *caller);
  static std::string load_profile(CommandCaller *caller, const std::string &fullFileName);
  static std::string disable_profile(CommandCaller *caller, const std::string &profileName);
  static std::string execute_change(CommandCaller *caller,
                                    const std::string &profile,
                                    const std::string &old_status,
                                    const std::string &new_status);

#ifdef TESTS_ENABLED
  FRIEND_TEST(CommandCallerTest, TEST_UNCONF);
  FRIEND_TEST(CommandCallerTest, TEST_STATUS);
  FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_EE);
  FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CC);
  FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CE);
  FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_EC_SUCCESS);
  FRIEND_TEST(CommandCallerTest, TEST_CHANGE_STATUS_CE_FAIL);
#endif
};

#endif // COMMAND_CALLER_H
