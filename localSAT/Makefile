CC=g++
CFLAGS=-c -Wall -g  -O3 
LDFLAGS=-g 
OBJECTS=main.o  miscellaneous.o 
EXECUTABLE=local

all:$(EXECUTABLE)
                
$(EXECUTABLE): $(OBJECTS)
	$(CC)  -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS)  $<
clean :
	rm  *.o
