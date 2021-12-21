# Standard compiler variables
CC = g++
CCFLAGS = -Wall -g

# Rules start here

z1863905_project2: z1863905_project2.o
	$(CC) -o z1863905_project2 $(CCFLAGS) z1863905_project2.o

z1863905_project2.o: z1863905_project2.cc
	$(CC) -c $(CCFLAGS) z1863905_project2.cc

clean:
	-rm *.o z1863905_project2
