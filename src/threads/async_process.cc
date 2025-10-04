#include <cassert>
#include <cstddef>
#include <glibmm/spawn.h>

#include "async_process.h"

AsyncProcess::AsyncProcess(const int pid, const int stdin_fd)
  : pid{ pid },
    stdin_fd{ stdin_fd }
{
  assert(valid());
}

AsyncProcess::~AsyncProcess()
{
  if (pid > 0)
    Glib::spawn_close_pid(pid);

  if (stdin_fd > 0)
    close(stdin_fd);
}

std::list<std::string> AsyncProcess::readlines()
{
  assert(valid());

  // Read data from descriptor into stringstream
  std::stringstream ss;

  const size_t bufferSize = 1024;
  char buffer[bufferSize];

  ssize_t bytesRead;
  while ((bytesRead = read(stdin_fd, buffer, sizeof(buffer))) > 0) {
    ss.write(buffer, bytesRead);
  }

  // Split stringstream into list of results
  std::list<std::string> results;
  std::string data = ss.str();
  size_t end_pos   = data.length();

  for (size_t start_pos = 0; start_pos < data.length(); start_pos = end_pos) {
    size_t end_pos = data.find_first_of('\n');
    if (end_pos == data.npos) {
      end_pos = data.length();
    }

    results.emplace_back(data.substr(start_pos, end_pos));
  }

  return results;
}

bool AsyncProcess::valid()
{
  return pid > 0 && stdin_fd > 0;
}
