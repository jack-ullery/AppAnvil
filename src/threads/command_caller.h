#ifndef SRC_THREADS_COMMAND_CALLER
#define SRC_THREADS_COMMAND_CALLER

#include <string>
#include <vector>

/** 
  * Calls commands on the terminal to be used by the rest of the program.
  * This is where AppAnvil actually interfaces with AppArmor. 
  * Most of these functions are called on the second thread.
  **/
class CommandCaller
{
public:
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
   * @brief Return the output of `dmesg`
   *
   * @details
   * Returns the output of `dmesg` to get a list of system logs.
   *
   * @returns std::string the raw output of `dmesg`
   */
  static std::string get_logs();

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
    loadprofile
  */
  static std::string load_profile(const std::string &fullFileName);

  static std::string disable_profile(const std::string &profileName);

protected:
  struct results {
    int exit_status = 0;
    std::string output;
    std::string error;
  };

  // Used to call command-line commands from `/usr/sbin`
  virtual results call_command(const std::vector<std::string> &command);
  virtual std::string call_command(const std::vector<std::string> &command, const std::string &return_on_error);

  // Dependency Injection: For unit testing
  static std::string get_status(CommandCaller *caller);
  static std::string get_logs(CommandCaller *caller);
  static std::string get_unconfined(CommandCaller *caller);
  static std::string load_profile(CommandCaller *caller, const std::string &fullFileName);
  static std::string disable_profile(CommandCaller *caller, const std::string &profileName);
  static std::string execute_change(CommandCaller *caller, const std::string &profile, const std::string &old_status,
                                    const std::string &new_status);
};

#endif // COMMAND_CALLER_H
