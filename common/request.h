#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

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
      parse_params_and_path_from_uri();
//      print_params();
//      std::cout << path() << std::endl;
   }
   static HTTPRequest make_request(int sockfd);
   std::string to_string() const {
      return get_str_from_method(method_) + " " + uri_ + " " + version_;
   }
   std::string uri() const {
      return uri_;
   }
   std::string path() const {
   	return path_;
   }
   std::string version() const {
      return version_;
   }
   HTTPMethod method() const {
      return method_;
   }
   const std::unordered_map<std::string, std::string>& http_params() const {
   	return params_;
   }
   void print_headers_map() const {
      for (const auto it : headers_) {
         std::cout << it.first << ": " << it.second << std::endl;
      }
   }
   void print_params() const {
      for (const auto it : params_) {
         std::cout << it.first << ": " << it.second << std::endl;
      }
   }
private:
   void parse_params_and_path_from_uri();
private:
   HTTPMethod method_;
   std::string uri_;
   std::string version_;
   std::string path_;
   std::unordered_map<std::string, std::string> headers_;
   std::unordered_map<std::string, std::string> params_;
};

