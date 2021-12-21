# Standard compiler variables
CC = g++
CCFLAGS = -Wall -g -lpthread

# Rules start here

assign5driver: assign5driver.o
	$(CC) -o assign5driver $(CCFLAGS) assign5driver.o

assign5driver.o: assign5driver.cpp
	$(CC) -c $(CCFLAGS) assign5driver.cpp

clean:
	-rm *.o assign5driver
