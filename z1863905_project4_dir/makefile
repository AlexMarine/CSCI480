# Standard compiler variables
CC = g++
CCFLAGS = -Wall -g -lpthread

# Rules start here

all: z1863905_project4_p1 z1863905_project4_p2

z1863905_project4_p1: z1863905_project4_p1.o
	$(CC) -o z1863905_project4_p1 $(CCFLAGS) z1863905_project4_p1.o

z1863905_project4_p1.o: z1863905_project4_p1.cc
	$(CC) -c $(CCFLAGS) z1863905_project4_p1.cc

z1863905_project4_p2: z1863905_project4_p2.o
	$(CC) -o z1863905_project4_p2 $(CCFLAGS) z1863905_project4_p2.o

z1863905_project4_p2.o: z1863905_project4_p2.cc
	$(CC) -c $(CCFLAGS) z1863905_project4_p2.cc

clean:
	-rm *.o z1863905_project4_p1 z1863905_project4_p2
