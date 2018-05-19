CC=g++
CFLAGS=-c -Wall -g -pg -O3 -fopenmp
LDFLAGS=-g -pg -fopenmp
OBJECTS=main.o  miscellaneous.o 
EXECUTABLE=local

all:$(EXECUTABLE)
                
$(EXECUTABLE): $(OBJECTS)
	$(CC)  -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS)  $<
clean :
	rm  *.o
