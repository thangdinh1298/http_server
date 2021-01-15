all: io_helper thread_pool main server

io_helper:
	gcc -o io_helper.o -c io_helper.c

thread_pool:
	g++ -o thread_pool.o -c thread_pool.cpp
main:
	g++ -o main.o -c main.cpp 
server:
	g++ -o server io_helper.o thread_pool.o main.o -lpthread

clean:
	rm *.o server
