#include <cassert>
#include <fcntl.h>
#include <glibmm/spawn.h>

#include "async_process.h"

AsyncProcess::AsyncProcess(const int pid, const int stdin_fd)
  : pid{ pid },
    stdin_fd{ stdin_fd }
{
  assert(valid());

  // Set the file descriptor to non-blocking
  int flags = fcntl(stdin_fd, F_GETFL, 0);
  fcntl(stdin_fd, F_SETFL, flags | O_NONBLOCK);
}

AsyncProcess::~AsyncProcess()
{
  if (pid > 0) {
    Glib::spawn_close_pid(pid);
  }

  if (stdin_fd > 0) {
    close(stdin_fd);
  }
}

std::list<std::string> AsyncProcess::readlines()
{
  assert(valid());

  // Read data from descriptor into stringstream
  std::stringstream ss;
  char buffer[1024];

  ssize_t bytesRead;
  while ((bytesRead = read(stdin_fd, buffer, sizeof(buffer))) > 0) {
    ss.write(buffer, bytesRead);
  }

  // Split stringstream into list of results
  std::list<std::string> results;
  std::string line;
  while (std::getline(ss, line, '\n')) {
    results.emplace_back(line);
  }

  return results;
}

bool AsyncProcess::valid()
{
  return pid > 0 && stdin_fd > 0;
}
