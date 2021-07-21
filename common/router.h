#include <unordered_map>

#include "common/request.h"
#include "common/response.h"

typedef void (*Task)(const HTTPRequest&, HTTPResponse&);
class Router {
public:
   void add_task(Task t, const std::string& path);
   Task get_task(const std::string& path) const;
private:
   std::unordered_map<std::string, Task> task_map_;
};
