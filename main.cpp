#include "prethreaded/prethreaded_server.h"
#include "prethreaded/conn_buffer.h"

int main() {
   PrethreadedServer server(4, 100);
   server.run();
}
