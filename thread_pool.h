#pragma once

#include <queue>
#include <thread>

class ThreadPool {
public:
   ThreadPool(unsigned thread_num=4, unsigned connection_buffer_len=100);
//   ~ThreadPool();
private:
   unsigned int max_connection_buffer_len_;
   std::queue</*HttpConnection*/int> connection_buffer_;
   std::vector<std::thread> thread_buffer_;
};
