#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "../controller/profiles_controller.h"
#include "status.h"

#include <memory>
#include <string>
#include <vector>

template<class ColumnRecord> class Profiles : public Status
{
public:
  explicit Profiles();
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

protected:
  // Signal handlers
  void change_status();

private:
  void default_change_fun(const std::string &a, const std::string &b, const std::string &c);
  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;
};

#endif // TABS_PROFILES_H
