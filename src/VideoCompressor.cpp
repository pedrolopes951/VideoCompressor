#include <iostream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib> // For std::system
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/VideoCompressor.h"
#include "../include/UtilsCompression.h"

using namespace Utils;

bool TwoPassCompressor::compress(const std::string &inputPath, const std::string &outputPath, int targetSizeMB)
{
    std::cout << "Reching here" << std::endl;
    
    double videoDuration = getVideoDuration(inputPath);
    int targetBitrate = calculateTargetBitrate(targetSizeMB, videoDuration);

    
    // First pass
    std::string command1 = "ffmpeg -i \"" + inputPath + "\" "
                                                        "-c:v mpeg4 -b:v " +
                           std::to_string(targetBitrate) + "k "
                                                           "-pass 1 -an -f rawvideo \\dev\\null";
        std::cout << command1 << std::endl;

    //system(command1.c_str());

    // Second pass
    std::string command2 = "ffmpeg -i \"" + inputPath + "\" "
                                                        "-c:v mpeg4 -b:v " +
                           std::to_string(targetBitrate) + "k "
                                                           "-pass 2 -c:a aac -b:a 128k \"" +
                           outputPath + "\"";

    std::cout << command2 << std::endl;
    return system(command2.c_str()) == 0;
}
bool TwoPassCompressor::compress(const std::string &directoryPath, int targetSizeMB)
{
    DIR *dir;
    struct dirent *entry;

    std::string outputDir = directoryPath + "\\Compressed";

    // Attempt to open the directory
    if ((dir = opendir(directoryPath.c_str())) == NULL)
    {
        perror("Failed to open directory");
        return false;
    }

    // Check if output directory exists, if not create it
    if ((dir = opendir(outputDir.c_str())) == NULL)
    {
        // First enter into the dir
        // Attempt to create the directory
        std::string mkdirCommand = "mkdir -p \"" + outputDir + "\"";
        system(mkdirCommand.c_str());
    }
    else
    {
        closedir(dir); // Close the directory if it already exists
    }
    
    dir = opendir(directoryPath.c_str());
    

    while ((entry = readdir(dir)) != NULL)
    {
        std::string filename = entry->d_name;
        std::cout << filename << std::endl;

        // Check for regular files by filename extension
        if (filename.find(".mp4") != std::string::npos)
        {
            std::string inputFilePath = directoryPath + "\\" + filename;
            
            std::string outputFilePath = outputDir + "\\" + filename;
            std::cout << inputFilePath << std::endl;
            std::cout << outputFilePath << std::endl;

            // Validate if it's a regular file via stat
            struct stat fileStat;
            if (stat(inputFilePath.c_str(), &fileStat) == 0)
            {
                if (S_ISREG(fileStat.st_mode))
                {
                    // Call the compression function
                        std::cout << "Reching here" << std::endl;

                    if (!compress(inputFilePath, outputFilePath, targetSizeMB))
                    {
                        std::cerr << "Failed to compress: " << inputFilePath << std::endl;
                        closedir(dir);
                        return false;
                    }
                }
            }
        }
    }
    closedir(dir); // Close the directory
    return true;
}