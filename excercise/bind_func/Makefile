CFLAGS=-I$(shell pwd) -std=c++11

all:bind_func

bind_func: bind_func.o callback.o	
	g++ -o $@ $^ $(CFLAGS)

bind_func.o: bind_func.cpp bind_func.hpp
	g++ -c bind_func.cpp $(CFLAGS)
	
callback.o: callback.c callback.h
	g++ -c callback.c
	
clean:
	rm -rf *.o bind_func
