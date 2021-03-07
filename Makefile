FLAGS=
all: server
debug: FLAGS += -g
debug: server

server: io_helper thread_pool main
	g++ ${FLAGS} -o server io_helper.o thread_pool.o main.o -lpthread

io_helper: io_helper.c io_helper.h
	gcc ${FLAGS} -c io_helper.c

thread_pool: thread_pool.cpp thread_pool.h
	g++ ${FLAGS} -c thread_pool.cpp

main: main.cpp
	g++ ${FLAGS} -c main.cpp

clean:
	rm *.o server
