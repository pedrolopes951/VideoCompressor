#pragma once
#include "../include/VideoCompressor.h"
#include "../include/IVideoCompressor.h"

/*
Concrete Strategy - concrete implementation of the IVideoCompressor interface that specifies how to perform video compression
*/
class TwoPassCompressor  : public IVideoCompressor {
public:
    bool compress(const std::string& inputPath, const std::string& outputPath, int targetSizeMB) override;
    bool compress(const std::string& directoryPath, int targetSizeMB) override;

};