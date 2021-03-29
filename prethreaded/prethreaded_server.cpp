#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include "prethreaded_server.h"
#include "plugin.h"
extern "C" {
#include "common/io_helper.h"
}

const size_t REQUEST_LINE_LENGTH = 1024;
const size_t MAXBUF = 8192;

void work(PrethreadedServer* server) {
   while (true) {
      auto conn = server->get_conn();
      auto req = HTTPRequest::make_request(conn);
      auto resp = HTTPResponse(conn);
      auto task = server->task();
      task(req, resp);
   }
}

PrethreadedServer::PrethreadedServer(unsigned thread_num, unsigned connection_buffer_len): 
                       conn_buffer_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t(work, this);
      thread_buffer_.push_back(std::move(t));
   }
   task_ = &Plugin::serve_static_file;
}
