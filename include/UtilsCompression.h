#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <array>


namespace Utils
{
    std::string exec(const char *cmd);
    double getVideoDuration(const std::string& inputPath);
    int calculateTargetBitrate(int targetSizeMB,int videoDuration);
}