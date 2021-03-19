#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "prethreaded/conn_buffer.h"

class ThreadPool {
public:
   ThreadPool(unsigned thread_num=4, unsigned connection_buffer_len=100);
   void submit_conn(conn_t conn) {
      conn_buffer_.push_conn(conn);
   }

private:
   ConnectionBuffer conn_buffer_;
   std::vector<std::thread> thread_buffer_;
};
