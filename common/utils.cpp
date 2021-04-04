#include <sstream>
#include <sys/socket.h>

#include "common/utils.h"

bool is_equals(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

std::vector<std::string> split_string(const std::string& s, char delim) {
   std::vector<std::string> result;
   std::stringstream ss(s);
   std::string token;
   while (getline(ss, token, delim)) {
      result.push_back(token);
   }
   return result;
}

std::string readline_or_max(int sockfd, int len) {
   char c;
   std::string line;
   line.reserve(len);
   int i = 0;

   while (i < len) {
      auto ret = recv(sockfd, &c, 1, 0); //TODO: make non-blocking call
      if (ret == 0) { //EOF
         return line;
      } else if (ret == 1) { //Read successful
         line.push_back(c);
         if (c == '\n') {
            return line;
         }
      } else { // Error
//         return line; //TODO:throw
         throw std::runtime_error("Error reading from socket");
      }
      i++;
   }
}
