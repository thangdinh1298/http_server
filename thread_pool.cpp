#include <iostream>
#include <cstring>

#include "thread_pool.h"
#include "io_helper.h"

ThreadPool::ThreadPool(unsigned thread_num, unsigned connection_buffer_len): 
                       max_connection_buffer_len_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t([this]() {
            char buf[1024];
            char response[] = "Hello World\n";
            while (true) {
               std::unique_lock<std::mutex> lk(mu_); //automatically unlock when goes out of scope
               while (connection_buffer_.empty()) {
                  cv_.wait(lk);
               }
               std::cout << "Child Thread: Buffer size: " << connection_buffer_.size() << std::endl;
               auto next_task = connection_buffer_.front();
               connection_buffer_.pop();
               
               //Write response
               sprintf(buf, ""
                   "HTTP/1.0 200 OK\r\n"
                   "Server: OSTEP WebServer\r\n"
                   "Content-Length: %d\r\n"
                   "Content-Type: %s\r\n\r\n",
                   strlen(response), "text");
               write_or_die (next_task, buf, strlen(buf));
               write_or_die (next_task, response, strlen(response));
            }
      });
      thread_buffer_.push_back(std::move(t));
   }
}
                       
