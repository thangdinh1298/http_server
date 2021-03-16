#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include "thread_pool.h"
extern "C" {
#include "io_helper.h"
}

const size_t REQUEST_LINE_LENGTH = 1024;
const size_t MAXBUF = 8192;

void handle_conn(int conn);
void request_error(int conn, int code, const char* msg);
void serve_static_file(int conn, std::string filename);

ThreadPool::ThreadPool(unsigned thread_num, unsigned connection_buffer_len): 
                       conn_buffer_(connection_buffer_len) {
   for (unsigned int i = 0; i < thread_num; i++) {
      std::thread t([this]() {
            while (true) {
               auto conn = conn_buffer_.get_conn();
					handle_conn(conn);
                
              // //Write response
              // sprintf(buf, ""
              //     "HTTP/1.0 200 OK\r\n"
              //     "Server: My WebServer\r\n"
              //     "Content-Length: %d\r\n"
              //     "Content-Type: %s\r\n\r\n",
              //     strlen(response), "text");
              // write_or_die (conn, buf, strlen(buf));
              // write_or_die (conn, response, strlen(response));
            }
      });
      thread_buffer_.push_back(std::move(t));
   }
}
                       
void handle_conn(int conn) {
	char buf[REQUEST_LINE_LENGTH]; //On the stack?
	readline_or_die(conn, buf, REQUEST_LINE_LENGTH);
	std::string request_line(buf);
	std::stringstream ss (request_line);
	std::string method, uri, version;
	ss >> method >> uri >> version;
	std::cout << uri << std::endl;

	if (method != "GET") { //TODO: Insensitive case comparison
		request_error(conn, 501, "Request type not implemented");
	}
	
	std::string filename = (uri.size() == 1 && uri[0] == '/') ? "index.html" : uri.substr(1, uri.size() - 1);
	serve_static_file(conn, filename);
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
