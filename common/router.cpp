#include "router.h"

void Router::add_task(Task t,const std::string& path) {
   if (task_map_.count(path)) {
   	throw std::runtime_error("Path for task already exists");
   }
   task_map_[path] = t;
}

Task Router::get_task(const std::string& path) const {
   if (task_map_.count(path)) {
   	return task_map_.at(path);
   }
   throw std::runtime_error("Task doesn't exist'");
}
