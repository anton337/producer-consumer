#ifndef WORKER_H
#define WORKER_H

#include <deque>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "semaphore.h"

using namespace boost;
using namespace std;

template<typename T, typename F> 
class worker {
private:
  semaphore * m_items;
  semaphore * m_spaces;
  mutex * m_mutex;
  deque<T*> * m_buffer;
  thread * m_thread;
  F m_work; // process the item

protected:
  semaphore * getItems() {
    return m_items;
  }
  semaphore * getSpaces() {
    return m_spaces;
  }
  mutex * getMutex() {
    return m_mutex;
  }
  deque<T*> * getBuffer() {
    return m_buffer;
  }

  virtual void run(F) = 0;

public:
  worker( semaphore * p_items
        , semaphore * p_spaces
        , mutex * p_mutex
        , deque<T*> * p_buffer
        , F p_work
        )
    : m_items(p_items)
    , m_spaces(p_spaces)
    , m_mutex(p_mutex)
    , m_buffer(p_buffer)
    , m_work(p_work)
    , m_thread(NULL)
  {

  }

  void start() {
    if(m_thread == NULL) {
      m_thread = new thread(bind(&worker::run, this, m_work));
    }
  }

  void wait() {
    if(m_thread != NULL) {
      m_thread->join();
    }
  }

};

#endif

