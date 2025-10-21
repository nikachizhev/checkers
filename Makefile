TARGET = checkers
TEST_TARGET = test_runner
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Werror -g

SRCS = src/main.cpp src/game.cpp src/board.cpp src/piece.cpp
TEST_SRCS = test/test.cpp src/board.cpp src/piece.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
test:
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
.PHONY: all test clean