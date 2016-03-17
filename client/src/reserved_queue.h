#ifndef _RESERVED_QUEUE_H_
#define _RESERVED_QUEUE_H_

#include <vector>

const int kReservedQueueDefaultSize = 100;

class ReservedQueue {
 public:
  ReservedQueue() : ReservedQueue(kReservedQueueDefaultSize) {}

  ReservedQueue(size_t size) {
    queue.resize(size);
    this->size = size;
    begin = 0;
    end = 0;
  }

  void CopyQueue(const ReservedQueue& other_queue) {
    Clear();
    size_t other_index = other_queue.begin;    
    while (other_index != other_queue.end) {
      Push(other_queue.queue[other_index]);
      other_index = (other_index + 1) % other_queue.size;
    }
  }

  void Clear() {
    begin = 0;
    end = 0;
  }

  void Push (index_t element) {
    if (IsFull()) {
      DoubleSize();
    }
    queue[end] = element;
    end = (end + 1) % size;
  }

  void Pop () {
    if (IsEmpty()) {
      exit(2);
    }
    begin = (begin + 1) % size;
  }

  index_t Top () {
    if (IsEmpty()) {
      exit(3);
    }
    return queue[begin];
  }

  bool IsFull() {
    // One sentinel cell.  
    return ((end + 1) % size) == begin;
  }

  bool IsEmpty() {    
    return end == begin;
  }

 private:
  void DoubleSize() {
    std::vector<index_t> new_queue(size * 2);
    size = size * 2;
    size_t index = begin, new_index = 0;
    while (index != end) {
      new_queue[new_index] = queue[index];
      ++new_index;
      index = (index + 1) % size;
    }
    std::swap(new_queue, queue);
    begin = 0;
    end = new_index;
  }

  size_t begin, end;
  std::vector<index_t> queue;
  size_t size;
};

#endif