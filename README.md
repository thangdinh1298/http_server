# Simple UNIX http server
This repo contains a simple implementation of a concurrent HTTP server for learning purposes.The implementation follows a pre-threaded design. Each thread picks up a request, handle it and then move on to the next one in line.

Only works on linux

# Compiling
Compile using `cmake`. From the project's root dir, run
```
mkdir build && cd build && cmake ../
```
After configuration has been completed, simply run
```
make
```

# Usage
Define your own handler. Full example in plugin.h
```
   void serve_static_file(const HTTPRequest& req, HTTPResponse& resp) {
      resp.set_header("Content-Type", "text/plain; charset=utf-8");
      std::string filename = req.http_params().at("file");

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
```
then plug it into the server before starting
```
int main() {
   PrethreadedServer server(4, 100);
   server.add_task(Plugin::func, "/get_num");
   server.add_task(Plugin::serve_static_file, "/get_file");
   server.run();
}
```
# Credits
This implementation utilizes some network code from [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/) to accept and bind sockets (io_helper.h/c). The rest was implemented by me
# Future plan
Employ the epoll so that a single thread can handle multiple requests concurrently. Benchmark and compare with the current implementation