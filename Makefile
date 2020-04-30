CXX = g++
CXXFLAGS = -Wall -g -pthread -lncurses

all: main

main: main.o Car.o DeliveryCar.o Numerable.o Stall.o Tank.o
	$(CXX) $(CXXFLAGS) -o main main.o Car.o DeliveryCar.o Numerable.o Stall.o Tank.o

Numerable.o: Numerable.h

Car.o: Car.h 

DeliveryCar.o: DeliveryCar.h 

Stall.o : Stall.h 

Tanko.o : Tank.h 

clean: 
	rm *.o main