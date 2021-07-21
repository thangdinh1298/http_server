#include "prethreaded/prethreaded_server.h"
#include "prethreaded/conn_buffer.h"
#include "prethreaded/plugin.h"

int main() {
   PrethreadedServer server(4, 100);
   server.add_task(Plugin::func, "/get_num");
   server.add_task(Plugin::serve_static_file, "/get_file");
   server.run();
}
