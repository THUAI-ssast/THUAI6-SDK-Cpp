# compile all cpp into one main executable
CC = g++
CFLAGS = -std=c++11 -lwinpthread -static
SOURCES = $(wildcard *.cpp sdk/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main.exe
INCLUDES = .

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDES)

clean:
	del main.exe sdk\*.o *.o