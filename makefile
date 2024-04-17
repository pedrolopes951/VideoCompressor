CFLAGS := -Wall -MMD -MP -g
CXX := g++
TEST_DIR := test
FFMPEG_DIR := /c/ffmpeg-7.0
SDL_PATH := /c/SDL2
IMGUI_PATH := /c/imgui

FFMPEG_LIB_DIRS := $(FFMPEG_DIR) $(FFMPEG_DIR)/libavcodec $(FFMPEG_DIR)/libavutil $(FFMPEG_DIR)/libavformat $(FFMPEG_DIR)/libswscale $(FFMPEG_DIR)/libavfilter

INCLUDES := -I$(FFMPEG_DIR) \
            -I$(FFMPEG_DIR)/libavcodec \
            -I$(FFMPEG_DIR)/libavutil \
            -I$(FFMPEG_DIR)/libavformat \
            -I$(FFMPEG_DIR)/libswscale \
            -I$(FFMPEG_DIR)/libavfilter \
            -I$(SDL_PATH)/include \
            -I$(IMGUI_PATH) \
            -I$(IMGUI_PATH)/backends

CXXFLAGS += $(INCLUDES)
LDFLAGS := $(foreach dir, $(FFMPEG_LIB_DIRS), -L$(dir)) -L$(SDL_PATH)/lib/x64
LDLIBS :=-lSDL2 -lSDL2main -lavcodec -lavformat -lavfilter -lswscale -lavutil 

IMGUI_SOURCES := $(IMGUI_PATH)/imgui.cpp \
                 $(IMGUI_PATH)/imgui_draw.cpp \
                 $(IMGUI_PATH)/imgui_widgets.cpp \
                 $(IMGUI_PATH)/imgui_tables.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdl2.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdlrenderer2.cpp


SRC_DIR := src
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(IMGUI_SOURCES)
SRCSTEST := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
OBJSTEST := $(SRCSTEST:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/test/%.o)
TEST_EXECUTABLE := $(BUILD_DIR)/runTests

EXECUTABLE := $(BUILD_DIR)/VideoCompressor

all: $(EXECUTABLE)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

tests: $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJS) $(OBJSTEST)  # Link test objs and regular objs excluding main.o
	mkdir -p $(@D)
	$(CXX) $^ -o $@
	./$(TEST_EXECUTABLE)

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

.PHONY: all clean
