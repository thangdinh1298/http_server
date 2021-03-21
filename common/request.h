#pragma once

#include <string>

enum class HTTPMethod {
   GET,
   POST,
   INVALID
};
HTTPMethod get_method_from_str(const std::string& method_str);
std::string get_str_from_method(HTTPMethod method);

class HTTPRequest {
public:
   HTTPRequest(HTTPMethod method, std::string uri, std::string version) :
   method_(method), uri_(uri), version_(version) {

   }
   static HTTPRequest make_request(int sockfd);
   std::string to_string() const {
      return get_str_from_method(method_) + " " + uri_ + " " + version_;
   }

private:
   HTTPMethod method_;
   std::string uri_;
   std::string version_;
};
