#include "../include/VideoCompressor.h"
#include "../include/UtilsCompression.h"

#include <iostream>
#include <sstream>

using namespace Utils;

bool TwoPassCompressor::compress(const std::string& inputPath, const std::string& outputPath, int targetSizeMB) {
        double videoDuration = getVideoDuration(inputPath);
        int targetBitrate = calculateTargetBitrate(targetSizeMB, videoDuration);

        
        //First pass
        std::string command1 = "ffmpeg -i \"" + inputPath + "\" "
                               "-c:v mpeg4 -b:v " + std::to_string(targetBitrate) + "k "
                               "-pass 1 -an -f rawvideo /dev/null";
        system(command1.c_str());

        //Second pass
        std::string command2 = "ffmpeg -i \"" + inputPath + "\" "
                               "-c:v mpeg4 -b:v " + std::to_string(targetBitrate) + "k "
                               "-pass 2 -c:a aac -b:a 128k \"" + outputPath + "\"";
        return system(command2.c_str()) == 0;
        return 0;
    }