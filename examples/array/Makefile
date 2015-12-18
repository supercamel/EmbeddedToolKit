CC=g++-4.9
CFLAGS=-c -g -Wall -Wextra -Werror -std=c++14 -I../../inc
LDFLAGS= -L../../lib -letk
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp)) 
EXECUTABLE=array

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	find . -name \*.o -execdir rm {} \;
	rm -f $(EXECUTABLE)

