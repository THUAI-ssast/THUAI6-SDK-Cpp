# compile all cpp into one main executable
CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bin.exe
INCLUDES = .

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDES)

clean:
	rm -f bin.exe sdk\*.o *.o