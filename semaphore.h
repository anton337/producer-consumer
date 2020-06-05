#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

using namespace boost;

class semaphore {

private:
  unsigned int m_count;
  mutex m_mutex;
  condition_variable m_condition;

public:

  semaphore(unsigned int initial_count)
    : m_count(initial_count)
  {

  }

  // ++
  void signal() {
    unique_lock<mutex> lock(m_mutex);
    ++m_count;
    m_condition.notify_one();
  }

  // --
  void wait() {
    unique_lock<mutex> lock(m_mutex);
    while (m_count == 0) {
      m_condition.wait(lock);
    }
    --m_count;
  }

};

#endif
