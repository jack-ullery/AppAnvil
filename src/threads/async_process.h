#ifndef SRC_THREADS_ASYNC_PROCESS_H
#define SRC_THREADS_ASYNC_PROCESS_H

#include <list>
#include <string>

class AsyncProcess
{
public:
  AsyncProcess(const int pid, const int stdin_fd);
  AsyncProcess() = default;

  ~AsyncProcess();

  /**
   * @brief Read newline separated lines (if there are any) from stdout
   *
   * @details If no data exists to be read from, return an empty vector.
   * If valid() is false, this has undefined behavior
   *
   * @return std::vector<std::string>
   */
  std::list<std::string> readlines();

  /**
   * @brief Whether this object can be read from
   *
   * @return true
   * @return false
   */
  bool valid();

  // Deletes copy/move constructors and operators
  AsyncProcess(const AsyncProcess &)            = delete;
  AsyncProcess(AsyncProcess &&)                 = delete;
  AsyncProcess &operator=(const AsyncProcess &) = delete;
  AsyncProcess &operator=(AsyncProcess &&)      = delete;

private:
  const int pid      = 0;
  const int stdin_fd = 0;
};

#endif