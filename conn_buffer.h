#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

typedef int conn_t;
class ConnectionBuffer {
private:
   unsigned int max_connection_buffer_len_;
   std::queue<int> connection_buffer_;
	std::mutex mu_;
	std::condition_variable cv_;

   bool is_full() {
      return connection_buffer_.size() == max_connection_buffer_len_;
   }
public:
   ConnectionBuffer(unsigned int len) : max_connection_buffer_len_(len) {

   }
   conn_t get_conn() {
      std::unique_lock<std::mutex> lk(mu_);
      while (connection_buffer_.empty()) {
         cv_.wait(lk);
      }
      auto conn = connection_buffer_.front();
      connection_buffer_.pop();
      return conn;
   }

   void push_conn(conn_t conn) {
      std::unique_lock<std::mutex> lk(mu_);
      while (is_full()) {
         cv_.wait(lk);
      }
      connection_buffer_.push(conn);
      cv_.notify_one();
   }
};
