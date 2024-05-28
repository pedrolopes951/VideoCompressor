#pragma once
#include "../include/IVideoCompressor.h"
#include <string>


/*
Context - anages the video compression strategy. It holds a reference to a strategy object 
and can change it at runtime depending on the user's choice
*/

class VideoCompressorContext {
private:
    IVideoCompressor* compressor{nullptr};

public:
    VideoCompressorContext(IVideoCompressor* comp);
    VideoCompressorContext() = default;
    ~VideoCompressorContext();

    void setCompressor(IVideoCompressor* comp);

    bool compressVideo(const std::string& input, const std::string& output, int targetSize);
    bool compressVideo(const std::string& directoryPath, int targetSize);

};
