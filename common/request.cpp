#include <iostream>

#include "common/request.h"
#include "common/utils.h"

#define BUFLEN 8196
#define MAX_REQUEST_LINE_SIZE 8196
#define MAX_HEADERS_SIZE 62

void parse_header(const std::string& header_line, 
                  std::unordered_map<std::string, std::string>& headers) {
   auto delim_pos = header_line.find(':');
   if (delim_pos == std::string::npos) {
      std::cout << header_line << std::endl;
      throw std::runtime_error("Ill-formed header");
   }
   auto key = header_line.substr(0, delim_pos);
   auto value = header_line.substr(delim_pos + 1, header_line.size() - delim_pos + 1);
   headers[key] = value;
}

HTTPRequest HTTPRequest::make_request(int sockfd) {
   std::string request_line = readline_or_max(sockfd, MAX_REQUEST_LINE_SIZE);
   if (request_line.find("\r\n") == std::string::npos) {
      throw std::runtime_error("request line exceeds 8kB");
   }
   auto tokens = split_string(request_line, ' ');
   if (tokens.size() != 3) {
      throw std::runtime_error("Invalid request line");
   }
   HTTPRequest req = {get_method_from_str(tokens[0]), tokens[1], tokens[2]};
   std::cout << req.to_string() << std::endl;

   unsigned bytes_read = 0;
   std::string header_line;
   while (MAX_HEADERS_SIZE > bytes_read) { //consider KMP here ?
		header_line = readline_or_max(sockfd, MAX_HEADERS_SIZE - bytes_read);
		bytes_read += header_line.size();
      if (header_line.back() != '\n') { //must have terminated by reaching byte limit
         throw std::runtime_error("Headers exceeds 8kB abc");
      } else { //terminated by reaching \n
         if (header_line == "\r\n") {
            break;
         } else {
            parse_header(header_line, req.headers_);
         }
      }
   }
   req.print_headers_map();
   return req;
}

HTTPMethod get_method_from_str(const std::string& method_str) {
   if (is_equals(method_str, "get")) {
      return HTTPMethod::GET;
   } else if (is_equals(method_str, "post")) {
      return HTTPMethod::POST;
   } else {
      return HTTPMethod::INVALID;
   }
}

std::string get_str_from_method(HTTPMethod method) {
   switch (method) {
      case HTTPMethod::GET: {
         return "GET";
      }
      case HTTPMethod::POST: {
         return "POST";
      }
      default: {
         return "INVALID";
      }
   }
}
