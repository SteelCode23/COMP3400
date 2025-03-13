CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -IUtilityServiceSystem

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

TARGET := $(BIN_DIR)/main

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET)