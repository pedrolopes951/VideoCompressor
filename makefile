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
            -I$(IMGUI_PATH)/backends

CXXFLAGS := -Wall -MMD -MP -g $(INCLUDES)
LDFLAGS := $(foreach dir, $(FFMPEG_LIB_DIRS), -L$(dir)) -L$(SDL_PATH)/lib/x64
LDLIBS := -lSDL2 -lSDL2main -lavcodec -lavformat -lavfilter -lswscale -lavutil 

IMGUI_SOURCES := $(IMGUI_PATH)/imgui.cpp \
                 $(IMGUI_PATH)/imgui_draw.cpp \
                 $(IMGUI_PATH)/imgui_widgets.cpp \
                 $(IMGUI_PATH)/imgui_tables.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdl2.cpp \
                 $(IMGUI_PATH)/backends/imgui_impl_sdlrenderer2.cpp

IMGUI_OBJS := $(IMGUI_SOURCES:.cpp=.o)

SRC_DIR := src
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(IMGUI_SOURCES)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

TEST_DIR := test

EXECUTABLE := $(BUILD_DIR)/VideoCompressor

all: $(EXECUTABLE)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

tests:
	cd $(TEST_DIR) && $(MAKE)

clean:
	rm -rf $(BUILD_DIR) $(DEPS)
	cd $(TEST_DIR) && $(MAKE) clean

-include $(DEPS)

.PHONY: all clean tests
