#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include "prethreaded_server.h"
extern "C" {
#include "common/io_helper.h"
}

const size_t REQUEST_LINE_LENGTH = 1024;
const size_t MAXBUF = 8192;

void handle_conn(int conn);
void request_error(int conn, int code, const char* msg);
void serve_static_file(int conn, std::string filename);

void func(const HTTPRequest& req, HTTPResponse& resp) {
	static int i = 12;
	return i * i++;
};

void work(PrethreadedServer* server) {
   auto conn = server->get_conn();
   auto req = HTTPRequest::make_request(conn);
   auto task = server->task();
   auto resp = task(req);
   std::cout << resp << std::endl;
}

PrethreadedServer::PrethreadedServer(unsigned thread_num, unsigned connection_buffer_len): 
                       conn_buffer_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t(work, this);
      thread_buffer_.push_back(std::move(t));
   }
   task_ = &func;
}
                       
void serve_static_file(int conn, std::string filename) {
	char buf[MAXBUF];
	std::fstream f(filename);

	if (!f.is_open()) {
		request_error(conn, 404, "Server cannot open file"); //Separate 404 and 403 cases
	}

	f.seekg (0, f.end);
   int length = f.tellg();
   f.seekg (0, f.beg);

	//write headers
	sprintf(buf, "HTTP/1.0 %d\r\n", 200);
	write_or_die (conn, buf, strlen(buf));

	sprintf(buf, "Content-Length: %d\r\n", length);
	write_or_die (conn, buf, strlen(buf));

	sprintf(buf, "Content-Type: html\r\n\r\n");
	write_or_die (conn, buf, strlen(buf));

	while (true) {
		f.read(buf, MAXBUF);
		int bytes_read = f.gcount();
		write_or_die (conn, buf, bytes_read);
		if (bytes_read < MAXBUF) {
			break;
		}
	}
}

void request_error(int conn, int code, const char* msg) {
	char buf[MAXBUF], body[MAXBUF];
	sprintf(body, "<head>\r\n"
						"<title>Concurrent Webserver Error</title>\r\n"
						"</head>\r\n"
						"<body>\r\n"
						"<p>%s</p>\r\n"
						"</body>\r\n");

	sprintf(buf, "HTTP/1.0 %d %s\r\n", code, msg);
	write_or_die (conn, buf, strlen(buf));

	sprintf(buf, "Content-Length: %d\r\n", strlen(body));
	write_or_die (conn, buf, strlen(buf));

	sprintf(buf, "Content-Type: html\r\n\r\n");
	write_or_die (conn, buf, strlen(buf));

	write_or_die (conn, body, strlen(body));
}
