#ifndef PRODUCER_H
#define PRODUCER_H

#include <deque>
#include <boost/thread/mutex.hpp>
#include "semaphore.h"
#include "worker.h"

using namespace boost;
using namespace std;

template<typename T>
class producer : public worker<T, T* (*)()> {
private:
  void run(T* (*produce)()) {
    while(true) {
      T * item = produce();
      this->getSpaces()->wait();
      {
        unique_lock<mutex> lock(*this->getMutex());
        this->getBuffer()->push_back(item);
      }
      this->getItems()->signal();
    }
  }

public:
  producer( semaphore * p_items
          , semaphore * p_spaces
          , mutex * p_mutex
          , deque<T*> * p_buffer
          , T* (*produce)()
          )
    : worker<T, T* (*)()> ( p_items
                          , p_spaces
                          , p_mutex
                          , p_buffer
                          , produce
                          )
  {

  }
};

#endif

