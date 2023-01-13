#ifndef SRC_AA_CALLER
#define SRC_AA_CALLER

#include <string>
#include <vector>

/**
 * Calls commands on the terminal to be used by the rest of the program.
 * This is where AppAnvil actually interfaces with AppArmor.
 * Most of these functions are called on the second thread.
 **/
class AppArmorCaller
{
public:
  // Default constructor and destructor
  AppArmorCaller()          = default;
  virtual ~AppArmorCaller() = default;

  // Copy/move constructors and operators
  AppArmorCaller(const AppArmorCaller &)            = default;
  AppArmorCaller(AppArmorCaller &&)                 = delete;
  AppArmorCaller &operator=(const AppArmorCaller &) = default;
  AppArmorCaller &operator=(AppArmorCaller &&)      = delete;

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
  static std::string get_status(AppArmorCaller *caller);
  static std::string get_unconfined(AppArmorCaller *caller);
};

#endif // COMMAND_CALLER_H
