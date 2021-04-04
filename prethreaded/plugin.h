#pragma once

#include <iostream>

#include "common/request.h"
#include "common/response.h"

namespace Plugin {
   void func(const HTTPRequest& req, HTTPResponse& resp) {
      static int i = 12;
      const auto& result = std::to_string(i);
      resp.set_header("Content-Type", "text");
      resp.set_header("Content-Length", "4");
      resp.write_to_body(result.data(), result.size());
      resp.write_to_body(result.data(), result.size());
   };

   void serve_static_file(const HTTPRequest& req, HTTPResponse& resp) {
      resp.set_header("Content-Type", "text/plain; charset=utf-8");
      std::string uri = req.uri();
      std::string filename = (uri.size() == 1 && uri[0] == '/') ?\
                             "index.html" : uri.substr(1, uri.size() - 1);

      std::fstream f(filename, std::ios::binary | std::ios::in);

      if (!f.is_open()) {
         resp.set_code(404);
         resp.set_msg("File can't be opened");
         resp.set_header("Content-Length", "0");
         return;
      }

      f.seekg (0, f.end);
      int length = f.tellg();
      f.seekg (0, f.beg);

      resp.set_header("Content-Length", std::to_string(length));

      char c;
      while (true) {
         f.read(&c, sizeof(char));
         if (!f) {
            break;
         }
         resp.write_to_body(&c, 1);
      }
   }
}
