INCLUDE_DIR = include
BUILD_DIR = bin
SRC_DIR = src

CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(BUILD_DIR)/$(TARGET) -I$(INCLUDE_DIR) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ $(LDFLAGS)

exec: all
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Juste pour me rappeler à quel point je perds mon temps 
lines:
	find $(SRC_DIR) $(INCLUDE_DIR) -name '*.cpp' -o -name '*.h' | xargs wc -l | cat

.PHONY: all exec clean lines exec
