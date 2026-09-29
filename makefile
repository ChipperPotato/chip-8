# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -std=c++17 -I include
# Linker flags
LDFLAGS := -lSDL3

TARGET := chip8

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
