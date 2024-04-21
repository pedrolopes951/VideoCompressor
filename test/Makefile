CXX := g++
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
            -I$(IMGUI_PATH)/backends \
            -I../src  # Include the project's main source directory for headers

CXXFLAGS := -Wall -MMD -MP -g $(INCLUDES)
LDFLAGS := $(foreach dir, $(FFMPEG_LIB_DIRS), -L$(dir)) -L$(SDL_PATH)/lib/x64
LDLIBS := -lSDL2 -lSDL2main -lavcodec -lavformat -lavfilter -lswscale -lavutil 

IMGUI_SOURCES := $(IMGUI_PATH)/imgui.cpp \
                 $(IMGUI_PATH)/imgui_draw.cpp \
                 $(IMGUI_PATH)/imgui_widgets.cpp \
                 $(IMGUI_PATH)/imgui_tables.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdl2.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdlrenderer2.cpp

IMGUI_OBJS := $(IMGUI_SOURCES:%.cpp=%.o)

BUILD_DIR := ./build
TEST_DIR := .
SRC_FILES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_EXECS := $(SRC_FILES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%)

all: $(TEST_EXECS)

$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(IMGUI_OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< $(IMGUI_OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) *.o *.d

-include $(BUILD_DIR)/*.d

.PHONY: all clean
