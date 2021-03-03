all: io_helper thread_pool main server
FLAGS=-g

io_helper:
	gcc -o io_helper.o -c io_helper.c

thread_pool:
	g++ ${FLAGS} -o thread_pool.o -c thread_pool.cpp
main:
	g++ ${FLAGS} -o main.o -c main.cpp 
server:
	g++ ${FLAGS} -o server io_helper.o thread_pool.o main.o -lpthread

clean:
	rm *.o server
