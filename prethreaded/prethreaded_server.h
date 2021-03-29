#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "prethreaded/conn_buffer.h"
#include "common/request.h"
#include "common/response.h"
extern "C" {
   #include "common/io_helper.h"
}

class PrethreadedServer {
public:
   typedef void (*Task)(const HTTPRequest&, HTTPResponse&);
public:
   PrethreadedServer(unsigned thread_num=4, unsigned connection_buffer_len=100);
   void run() {
      int listen_fd = open_listen_fd_or_die(6969);
      for (;;) {
         struct sockaddr_in client_addr;
         int client_len = sizeof(client_addr);
         int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
         conn_buffer_.push_conn(conn_fd);
      }
   }
   Task task() const {
      return task_;
   }
   int get_conn() {
      return conn_buffer_.get_conn();
   }
private:
   ConnectionBuffer conn_buffer_;
   std::vector<std::thread> thread_buffer_;
   Task task_;
};
