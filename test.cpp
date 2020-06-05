#include <stdio.h>
#include <vector>
#include "producer.h"
#include "consumer.h"

using namespace boost;
using namespace std;

int* produce() {
  int * i = new int(rand());
  return i;
}

void consume(int* num) {
  int x = *num;
  printf("%d\n", x);
  delete num;
}

int main() {

  deque<int*>* Q = new deque<int*>();
  const size_t BUFFER_SIZE = 1000;
  mutex * M = new mutex();
  semaphore * items = new semaphore(0);
  semaphore * spaces = new semaphore(BUFFER_SIZE);

  vector<producer<int>*> Producers;
  for(int i=0;i<300;i++) {
    Producers.push_back(new producer<int>(items, spaces, M, Q, produce));
  }

  vector<consumer<int>*> Consumers;
  for(int i=0;i<300;i++) {
    Consumers.push_back(new consumer<int>(items, spaces, M, Q, consume));
  }

  for(int i=0;i<Producers.size();i++) {
    Producers[i]->start();
  }

  for(int i=0;i<Consumers.size();i++) {
    Consumers[i]->start();
  }

  for(int i=0;i<Producers.size();i++) {
    Producers[i]->wait();
  }

  for(int i=0;i<Consumers.size();i++) {
    Consumers[i]->wait();
  }

  return 0;

}

