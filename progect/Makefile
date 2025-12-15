.PHONY: all
.PHONY: build
.PHONY: debug
.PHONY: tests
.PHONY: run-tests
.PHONY: run
.PHONY: run-dbg
.PHONY: clean

override CPPFILES := $(shell find  -L src/* -type f -name '*.cpp')
override OBJ := $(addprefix obj/,$(CPPFILES:.cpp=.cpp.o))

TARGET =  server
CC = g++
LIBS = -lcrypto++ 
CXXINCLUDE = include/
CXXFLAGS = 	-std=c++17                     \

all:build

run: build
	./$(TARGET)

run-dbg: debug
	./$(TARGET)_dbg

build: CXXFLAGS+=-O3
build: $(OBJ)
	$(CC) $(CXXFLAGS) -I $(CXXINCLUDE) -g  -c main.cpp -o main.cpp.o  
	$(CC) -I $(CXXINCLUDE) $(OBJ) main.cpp.o $(LDFLAGS) -o $(TARGET) $(LIBS)

debug: CXXFLAGS+=-g -O0
debug: $(OBJ)
	$(CC) $(CXXFLAGS) -I $(CXXINCLUDE)  -g  -c main.cpp -o main.cpp.o 
	$(CC) -I $(CXXINCLUDE)  $(OBJ) main.cpp.o -g  -o $(TARGET)_dbg $(LIBS)

tests: CXXFLAGS+=-g -O0
tests: $(OBJ)
	$(CC)  -g $(CXXFLAGS) -I $(CXXINCLUDE) -c tests.cpp -o   tests.cpp.o 
	$(CC)  -g -I $(CXXINCLUDE)  $(OBJ) tests.cpp.o -o tests $(LIBS) -lUnitTest++ 
	
run-tests: tests
	./tests

obj/%.cpp.o: %.cpp
	mkdir -p "$$(dirname $@)"
	$(CC) $(CXXFLAGS) -I  $(CXXINCLUDE) -c $< -o $@

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f $(TARGET)_dbg
	rm -f tests