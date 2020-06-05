#ifndef CONSUMER_H
#define CONSUMER_H

#include <deque>
#include <boost/thread/mutex.hpp>
#include "semaphore.h"
#include "worker.h"

using namespace boost;
using namespace std;

template<typename T>
class consumer : public worker<T, void (*)(T*)> {
private:
  void run(void (*consume)(T*)) {
    while(true) {
      T * item = NULL;
      this->getItems()->wait();
      {
        unique_lock<mutex> lock(*this->getMutex());
        if(this->getBuffer()->empty() == false) {
          item = this->getBuffer()->front();
          this->getBuffer()->pop_front();
        }
      }
      this->getSpaces()->signal();
      if(item != NULL) {
        (*consume)(item);
      }
    }
  }

public:
  consumer( semaphore * p_items
          , semaphore * p_spaces
          , mutex * p_mutex
          , deque<T*> * p_buffer
          , void (*consume)(T*)
          )
    : worker<T, void (*)(T*)> ( p_items
                              , p_spaces
                              , p_mutex
                              , p_buffer
                              , consume
                              )
  {

  }
};

#endif
