TARGET = checkers
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Werror

SRCS = src/main.cpp src/game.cpp src/board.cpp src/piece.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
