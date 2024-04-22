#include "../include/VideoCompressorContext.h"
#include <iostream>

VideoCompressorContext::VideoCompressorContext(IVideoCompressor *comp) : compressor(comp) {}
VideoCompressorContext::~VideoCompressorContext() { delete compressor; }

void VideoCompressorContext::setCompressor(IVideoCompressor *comp)
{
    if(compressor != nullptr)
    {
        delete compressor;
    }
    compressor = comp;
}

bool VideoCompressorContext::compressVideo(const std::string &input, const std::string &output, int targetSize)
{
    
    return compressor->compress(input, output, targetSize);
}