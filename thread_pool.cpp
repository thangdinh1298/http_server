#include <iostream>
#include "thread_pool.h"

ThreadPool::ThreadPool(unsigned thread_num, unsigned connection_buffer_len): 
                       max_connection_buffer_len_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t([this]() {
				std::cout << "Performing" << std::endl;
				std::unique_lock<std::mutex> lk(mu_); //automatically unlock when goes out of scope
				cv_.wait(lk, [this](){return !connection_buffer_.empty();});
				auto next_task = connection_buffer_.front();
				connection_buffer_.pop();
				std::cout << next_task << std::endl;
      });
      thread_buffer_.push_back(std::move(t));
   }
}
                       
