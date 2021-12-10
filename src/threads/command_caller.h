#ifndef SRC_THREADS_COMMAND_CALLER
#define SRC_THREADS_COMMAND_CALLER

#include <string>

class CommandCaller{
  public:
    /**
     * @brief Return the output of `aa-status --json`
     * 
     * @details
     * Returns the output of `pkexec aa-status --json` to get a list of profiles and processes confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-status
     */
    static std::string get_status_str();

    /**
     * @brief Return the output of `dmesg`
     * 
     * @details
     * Returns the output of `dmesg` to get a list of system logs. 
     * 
     * @returns std::string the raw output of `dmesg`
     */
    static std::string get_logs_str();

    /**
     * @brief Return the output of `aa-unconfined`
     * 
     * @details
     * Returns the output of `pkexec aa-unconfined` to get a list of processes not confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-unconfined
     */
    static std::string get_unconfined();

    /*
      loadprofile
    */
    static std::string load_profile(std::string fullFileName);

    /**
     * @brief Sets the given profile to the given status
     * 
     * @param profileName- The name of the profile that will be changed. This appears in the 'profiles' tab.
     * @param status- The status that the profile will be changed to. Can be aa-complain, aa-enforce, and possibly aa-disable
     *  
     * 
     * @returns std::string the raw output of the command. Returns any error messages AppArmor gives
     */
    static std::string set_status(std::string profileName, std::string status);
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
    static std::string execute_change(const std::string& profile, const std::string& old_status, const std::string& new_status);
};

#endif // COMMAND_CALLER_H
