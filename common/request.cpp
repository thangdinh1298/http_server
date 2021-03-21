#include <iostream>

#include "common/request.h"
#include "common/utils.h"

#define BUFLEN 8196
#define MAX_REQUEST_LINE_SIZE 8196
#define MAX_HEADERS_SIZE 8196

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
