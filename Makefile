CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

TARGET = campusGuard

SOURCES = $(wildcard *.cpp)
OBJ = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

valgrind: $(TARGET)
	valgrind --leak-check=full -s --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)



