CXX = g++
CXXFLAGS = -std=c++11 -Iinclude/ -g
LIBS = -lcryptopp -lpthread

# Объектные файлы для сервера
SERVER_OBJS = main.o Server.o CmdLine.o Logger.o UsersParser.o Communication.o

# Объектные файлы для тестов  
TEST_OBJS = tests.o Server.o CmdLine.o Logger.o UsersParser.o Communication.o

all: server

server: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o server $^ $(LIBS)

tests: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o tests $^ $(LIBS) -lUnitTest++

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o server tests

run: server
	./server

test: tests
	./tests
