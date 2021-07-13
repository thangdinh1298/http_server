cmake_minimum_required(VERSION 3.2)
project(http_server)

find_package(Threads REQUIRED)
include_directories(${CMAKE_SOURCE_DIR})
add_library(utils common/utils.cpp)

add_library(request common/request.cpp)
target_link_libraries(request
	PRIVATE utils	
)

add_library(io_helper common/io_helper.c)
add_library(prethreaded_server prethreaded/prethreaded_server.cpp)
target_link_libraries(prethreaded_server
	PRIVATE io_helper request Threads::Threads
)

add_executable(server main.cpp)
target_link_libraries(server 
	PRIVATE prethreaded_server
)
