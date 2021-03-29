#pragma once

#include <map>

class HTTPResponse {
public:
   HTTPResponse() = delete;
   HTTPResponse(int fd) {
      FILE* fp = fdopen(fd, "w");
      body_ = fp;
   }
   HTTPResponse(FILE* body) : body_(body) {}
   ~HTTPResponse() {
      if (!status_and_headers_sent_) {
         write_status_and_header();
      }
      fclose(body_);
   }
   size_t write_to_body(const char* buf, size_t len) {
      if (!status_and_headers_sent_) {
         write_status_and_header();
         status_and_headers_sent_ = true;
      }
      return fwrite(buf, sizeof(char), len, body_);
   }
   void set_code(int code) {
   	code_ = code;
   }
   void set_msg(std::string msg) {
   	msg_ = msg;
   }
   void set_header(std::string key, std::string value) {
      headers_[key] = value;
   }
private:
   void write_status_and_header() { //TODO:Handle fwrite errors
      std::string status_line = version_ + " "  + std::to_string(code_)\
                                  + " " + msg_ + "\r\n";
      fwrite(status_line.data(), sizeof(char), status_line.size(), body_);
      for (const auto& p : headers_) {
         std::string line = p.first + ": " + p.second + "\r\n";
         fwrite(line.data(), sizeof(char), line.size(), body_);
      }
      fwrite("\r\n", sizeof(char), 2, body_);
   }
private:
	std::string msg_="OK";
	int code_=200;
	std::string version_="HTTP/1.0";
	FILE* body_ = nullptr; //buffered_io
   bool status_and_headers_sent_ = false;
   std::map<std::string, std::string> headers_;
};
