#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "prethreaded/conn_buffer.h"
extern "C" {
   #include "common/io_helper.h"
}

class PrethreadedServer {
public:
   PrethreadedServer(unsigned thread_num=4, unsigned connection_buffer_len=100);
   void submit_conn(int conn) {
      conn_buffer_.push_conn(conn);
   }
   void run() {
      int listen_fd = open_listen_fd_or_die(6969);
      for (;;) {
         struct sockaddr_in client_addr;
         int client_len = sizeof(client_addr);
         int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
         submit_conn(conn_fd);
      }
   }

private:
   ConnectionBuffer conn_buffer_;
   std::vector<std::thread> thread_buffer_;
};
