INCLUDE_DIR = include
BUILD_DIR = bin
SRC_DIR = src

CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

TARGET = app

all: shaders $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(BUILD_DIR)/$(TARGET) -I$(INCLUDE_DIR) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ $(LDFLAGS)

run: all
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Juste pour me rappeler à quel point je perds mon temps 
lines:
	find $(SRC_DIR) $(INCLUDE_DIR) res/shaders -name '*.cpp' -o -name '*.h' -o -name '*.glsl' | xargs wc -l | cat

shaders:
	glslc -fshader-stage=comp res/shaders/comp.glsl -o res/shaders/comp.spv
	glslc -fshader-stage=vert res/shaders/vert.glsl -o res/shaders/vert.spv
	glslc -fshader-stage=frag res/shaders/frag.glsl -o res/shaders/frag.spv

debug: all
	gdb $(BUILD_DIR)/$(TARGET)

test:
	g++ -o bin/test src/main.cpp
	./bin/test

.PHONY: all run clean lines shaders debug test
