FLAGS=
INCLUDE_DIR=.
all: prethreaded_server.o io_helper.o main.o request.o utils.o server
debug: FLAGS += -g
debug: prethreaded_server io_helper main server

utils.o: common/utils.h common/utils.cpp
	g++ ${FLAGS} -I ${INCLUDE_DIR} -c common/utils.cpp

request.o: common/request.h common/request.cpp utils.o
	g++ ${FLAGS} -I ${INCLUDE_DIR} -c common/request.cpp

prethreaded_server.o: prethreaded/prethreaded_server.h prethreaded/prethreaded_server.cpp prethreaded/conn_buffer.h io_helper.o
	g++ ${FLAGS} -I ${INCLUDE_DIR} -c prethreaded/prethreaded_server.cpp

io_helper.o: common/io_helper.c common/io_helper.h
	gcc ${FLAGS} -I ${INCLUDE_DIR} -c common/io_helper.c

main.o: main.cpp
	g++ ${FLAGS} -I ${INCLUDE_DIR} -c main.cpp

server: main.o io_helper.o prethreaded_server.o request.o
	g++ ${FLAGS} -o server main.o io_helper.o prethreaded_server.o request.o utils.o -lpthread

clean:
	rm *.o server
