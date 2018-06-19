CC=g++
CFLAGS=-c -g -Wall -Wextra -fno-strict-overflow -Wstrict-overflow=5 -std=c++11 -I./inc -I/usr/include/eigen3
LDFLAGS=
SOURCES=$(wildcard src/*.cpp) $(wildcard tests/*.cpp)
HEADERS=$(wildcard inc/etk/*.h)
OBJECTS=$(patsubst src/%.cpp,.obj/%.o,$(wildcard src/*.cpp))
TEST_OBJECTS=$(patsubst tests/%.cpp,.obj/tests/%.o,$(wildcard tests/*.cpp))
EXECUTABLE=bin/main

all: inc/etk/version.h $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(TEST_OBJECTS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST_OBJECTS) -o $@
	./bin/main

.obj/%.o:src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.obj/tests/%.o:tests/%.cpp
	mkdir -p .obj/tests
	$(CC) $(CFLAGS) $< -o $@

inc/etk/version.h:
	./autoversion

clean:
	find . -name \*.o -execdir rm {} \;
	rm -f $(EXECUTABLE)
	rm inc/etk/version.h

install:
	cp -rf ./inc/etk /mingw32/include
