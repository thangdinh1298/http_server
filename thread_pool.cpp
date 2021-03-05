#include <iostream>
#include <cstring>

#include "thread_pool.h"
#include "io_helper.h"

ThreadPool::ThreadPool(unsigned thread_num, unsigned connection_buffer_len): 
                       conn_buffer_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t([this]() {
            char buf[1024];
            char response[] = "Hello World\n";
            while (true) {
               auto conn = conn_buffer_.get_conn();
                
               //Write response
               sprintf(buf, ""
                   "HTTP/1.0 200 OK\r\n"
                   "Server: My WebServer\r\n"
                   "Content-Length: %d\r\n"
                   "Content-Type: %s\r\n\r\n",
                   strlen(response), "text");
               write_or_die (conn, buf, strlen(buf));
               write_or_die (conn, response, strlen(response));
            }
      });
      thread_buffer_.push_back(std::move(t));
   }
}
                       
