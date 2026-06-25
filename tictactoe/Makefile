CXX      = g++
CXXFLAGS = -Wall -std=c++11

OBJS   = main.o board.o player.o ai.o game.o
TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp game.h
	$(CXX) $(CXXFLAGS) -c main.cpp

board.o: board.cpp board.h
	$(CXX) $(CXXFLAGS) -c board.cpp

player.o: player.cpp player.h
	$(CXX) $(CXXFLAGS) -c player.cpp

ai.o: ai.cpp ai.h board.h
	$(CXX) $(CXXFLAGS) -c ai.cpp

game.o: game.cpp game.h board.h player.h ai.h
	$(CXX) $(CXXFLAGS) -c game.cpp

clean:
	rm -f $(OBJS) $(TARGET)
