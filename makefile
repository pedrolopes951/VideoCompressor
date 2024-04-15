FFMPEG_DIR := /c/ffmpeg-7.0
FFMPEG_LIB_DIRS := $(FFMPEG_DIR) $(FFMPEG_DIR)/libavcodec $(FFMPEG_DIR)/libavutil $(FFMPEG_DIR)/libavformat $(FFMPEG_DIR)/libswscale $(FFMPEG_DIR)/libavfilter

CFLAGS := -Wall \
          -I$(FFMPEG_DIR) \
          -I$(FFMPEG_DIR)/libavcodec \
          -I$(FFMPEG_DIR)/libavutil \
          -I$(FFMPEG_DIR)/libavformat \
          -I$(FFMPEG_DIR)/libswscale \
          -I$(FFMPEG_DIR)/libavfilter

LDFLAGS := $(foreach dir, $(FFMPEG_LIB_DIRS), -L$(dir))

LDLIBS := -lavcodec -lavformat -lavfilter -lswscale -lavutil

SRC := main.cpp
BUILD_DIR := build
OBJ := $(BUILD_DIR)/$(SRC:.cpp=.o)
EXECUTABLE := $(BUILD_DIR)/VideoCompressor

all: $(EXECUTABLE)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	g++ $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ)
	mkdir -p $(BUILD_DIR)
	g++ $(LDFLAGS) $(OBJ) -o $@ $(LDLIBS)

clean:
	rm -rf $(BUILD_DIR)
