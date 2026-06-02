CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = 3122a_KA_Biblioteca
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET):$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

server: server_cpp.cpp
	$(CXX) -std=c++17 -O2 server_cpp.cpp -o server_cpp -lpthread

server-ssl: server_cpp.cpp
	$(CXX) -std=c++17 -O2 -DHAVE_SSL server_cpp.cpp -o server_cpp -lssl -lcrypto -lpthread

run-server: server
	./server_cpp

clean:
	rm -f src/*.o $(TARGET) server_cpp
