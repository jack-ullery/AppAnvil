
#include "status_controller_mock.h"
#include <memory>

StatusControllerMock::StatusControllerMock()
  : status_mock{ std::make_shared<StatusMock>() }
{
}

std::shared_ptr<StatusMock> StatusControllerMock::get_tab()
{
  return status_mock;
}