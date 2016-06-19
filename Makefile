CC=g++
CFLAGS=-c -g -Wall -Wextra -std=c++14 -I./inc -I/usr/include/eigen3
LDFLAGS= -lc
SOURCES=$(wildcard src/*.cpp) $(wildcard tests/*.cpp)
HEADERS=$(wildcard inc/etk/*.h)
OBJECTS=$(patsubst src/%.cpp,.obj/%.o,$(wildcard src/*.cpp))
TEST_OBJECTS=$(patsubst tests/%.cpp,.obj/tests/%.o,$(wildcard tests/*.cpp))
EXECUTABLE=bin/main

all: inc/etk/version.h $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(TEST_OBJECTS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST_OBJECTS) -o $@
	#./bin/main
	ar rcs libetk.a $(OBJECTS)

.obj/%.o:src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.obj/tests/%.o:tests/%.cpp
	mkdir -p .obj/tests
	$(CC) $(CFLAGS) $< -o $@

inc/etk/version.h:
	.obj/autoversion

clean:
	find . -name \*.o -execdir rm {} \;
	rm -f $(EXECUTABLE)
	rm inc/etk/version.h

install:
	cp -rf ./inc/etk /usr/include
	cp libetk.a /usr/lib
