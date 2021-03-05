#include "thread_pool.h"
#include "conn_buffer.h"
extern "C" {
   #include "io_helper.h"
}
#include <iostream>

int main() {
   ThreadPool pool(4, 100);
   int listen_fd = open_listen_fd_or_die(6969);
   for (;;) {
      struct sockaddr_in client_addr;
      int client_len = sizeof(client_addr);
      int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
		pool.submit_conn(conn_fd);
   }
}
