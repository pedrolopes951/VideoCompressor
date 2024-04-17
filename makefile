CFLAGS := -Wall -MMD -MP -g
CXX := g++
TEST_DIR := test
FFMPEG_DIR := /c/ffmpeg-7.0
FFMPEG_LIB_DIRS := $(FFMPEG_DIR) $(FFMPEG_DIR)/libavcodec $(FFMPEG_DIR)/libavutil $(FFMPEG_DIR)/libavformat $(FFMPEG_DIR)/libswscale $(FFMPEG_DIR)/libavfilter

INCLUDES := -I$(FFMPEG_DIR) \
            -I$(FFMPEG_DIR)/libavcodec \
            -I$(FFMPEG_DIR)/libavutil \
            -I$(FFMPEG_DIR)/libavformat \
            -I$(FFMPEG_DIR)/libswscale \
            -I$(FFMPEG_DIR)/libavfilter

LDFLAGS := $(foreach dir, $(FFMPEG_LIB_DIRS), -L$(dir))
LDLIBS := -lavcodec -lavformat -lavfilter -lswscale -lavutil

SRC_DIR := src
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCSTEST := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
OBJSTEST := $(SRCSTEST:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/test/%.o)
TEST_EXECUTABLE := $(BUILD_DIR)/runTests

EXECUTABLE := $(BUILD_DIR)/VideoCompressor

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

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
