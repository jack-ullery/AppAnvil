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
   * @brief Return the output of `ps`
   *
   * @details
   * Returns the output of `pkexec ps -A --format pid,ppid,user,context,comm` to get a
   * list of processes which may (or may not) ne confined by apparmor.
   *
   * @returns std::string the raw output of aa-unconfined
   */
  static std::string get_unconfined();

  /**
   * @brief Return the output of `ausearch`
   *
   * @details
   * Returns the output of `pkexec ausearch --raw -m AVC --checkpoint [filename]` to get a list of logs that may pertain to AppArmor,
   * where [filename] is a randomly named checkpoint file in /tmp.
   *
   * @returns std::string the raw output of ausearch, prepended by [filename]
   */
  static std::string get_logs();

  /**
   * @brief Return the output of `ausearch`
   *
   * @details
   * Returns the output of `pkexec ausearch --raw -m AVC --checkpoint [filename]` to get a list of logs that may pertain to AppArmor,
   * where [filename] is the argument to the method.
   *
   * @returns std::string the raw output of ausearch
   */
  static std::string get_logs(const std::string &filename);

protected:
  struct results
  {
    int exit_status = 0;
    std::string output;
    std::string error;
  };

  static constexpr uint COMMAND_NOT_FOUND_STATUS = 127;
  static constexpr uint AUSEARCH_NO_RESULT_STATUS = 256;

  // Used to call command-line commands from `/usr/sbin`
  virtual results call_command(const std::vector<std::string> &command);
  virtual std::string call_command(const std::vector<std::string> &command, const std::string &return_on_error);

  // Dependency Injection: For unit testing
  static std::string get_status(AppArmorCaller *caller);
  static std::string get_unconfined(AppArmorCaller *caller);
  static std::string get_logs(AppArmorCaller *caller, const std::string &filename);
};

#endif // COMMAND_CALLER_H
