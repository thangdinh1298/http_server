#include <iostream>
#include "thread_pool.h"

ThreadPool::ThreadPool(unsigned thread_num, unsigned connection_buffer_len): 
                       max_connection_buffer_len_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t([](unsigned int i) {
            std::cout << "Thread " << i << ": Hello World\n";
      }, i);
      thread_buffer_.push_back(std::move(t));
   }
}
                       
