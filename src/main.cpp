#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <array>
#include "../include/VideoCompressor.h"
extern "C"
{
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
}

static void printVersionLibs()
{
    std::cout << "libavutil version: " << AV_VERSION_MAJOR(avutil_version()) << "."
              << AV_VERSION_MINOR(avutil_version()) << "."
              << AV_VERSION_MICRO(avutil_version()) << std::endl;

    std::cout << "libavcodec version: " << AV_VERSION_MAJOR(avcodec_version()) << "."
              << AV_VERSION_MINOR(avcodec_version()) << "."
              << AV_VERSION_MICRO(avcodec_version()) << std::endl;

    std::cout << "libavformat version: " << AV_VERSION_MAJOR(avformat_version()) << "."
              << AV_VERSION_MINOR(avformat_version()) << "."
              << AV_VERSION_MICRO(avformat_version()) << std::endl;

    std::cout << "libswscale version: " << AV_VERSION_MAJOR(swscale_version()) << "."
              << AV_VERSION_MINOR(swscale_version()) << "."
              << AV_VERSION_MICRO(swscale_version()) << std::endl;

    std::cout << "libavfilter version: " << AV_VERSION_MAJOR(avfilter_version()) << "."
              << AV_VERSION_MINOR(avfilter_version()) << "."
              << AV_VERSION_MICRO(avfilter_version()) << std::endl;
}

// Function to execute a shell command and capture the output.
std::string exec(const char* cmd) {
    std::array<char, 128> buffer; // Buffer to read the output
    std::string result; 
    // Pipe to send the std out of the process opened by the popen if the duration of the video.decltype(&pclose) is a explicit deleter function to handle the popen
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose); // Safely manages the file pointer that interacts with the pipe "r"
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) { // Get the data from the pipe into the buffer 
        result += buffer.data();
    }
    return result;
}

// Function to get the duration of the video in seconds
double getVideoDuration(const std::string& inputPath) {
    std::string cmd = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"" + inputPath + "\""; // ffprobe to fetch the duration of the video
    std::string output = exec(cmd.c_str());
    return std::stod(output);
}
/// @brief function calculates how many bits per second (bps) you can use for the video part, aiming to meet a desired file size (in MB) over a given duration (in seconds)
/// @param fileSizeMB Desired file size of the video to compress
/// @param durationSeconds Duration of the original video  
/// @return return the adjusted bit rate for the video compressed
int calculateTargetBitrate(int fileSizeMB, double durationSeconds) {
    if (durationSeconds == 0)
        return 0;  // Prevent division by zero
    int totalBits = fileSizeMB * 8192; // 8 bits in a byte, 1024 bytes in a kilobyte
    int audioBits = 128 * durationSeconds; // Assuming 128 kbps audio bitrate
    return (totalBits - audioBits) / durationSeconds; // Adjust bitrate for video only
}

bool compress_video(const std::string& inputPath, const std::string& outputPath, int targetSizeMB) {
    double videoDuration = getVideoDuration(inputPath);
    int targetBitrate = calculateTargetBitrate(targetSizeMB, videoDuration);

    /*
    This uses an two way pass:
    First Pass - In the first pass, the encoder analyzes the video without actually producing the final encoded output. Saves the analysis in the/dev/null
    Second Pass - Using the data collected during the first pass, the second pass then encodes the video with optimized bit allocation,
    using the information from the first pass to make more informed decisions about bit usage 
    */
    // First pass (logs to null, no audio)
    std::string command1 = "ffmpeg -i \"" + inputPath + "\" "
                           "-c:v mpeg4 " // video codec alowed 
                           "-b:v " + std::to_string(targetBitrate) + "k " // Pass the video bitrate calculated
                           "-pass 1 -an -f rawvideo /dev/null";
    system(command1.c_str());

    // Second pass (actual encoding with audio)
    std::string command2 = "ffmpeg -i \"" + inputPath + "\" "
                           "-c:v mpeg4 "
                           "-b:v " + std::to_string(targetBitrate) + "k "
                           "-pass 2 "
                           "-c:a aac -b:a 128k "
                           "\"" + outputPath + "\"";
    return system(command2.c_str()) == 0; // Execute the command
}


int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input video path> <output video path> <target size in MB>\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int targetSizeMB = std::stoi(argv[3]);

    if (!compress_video(inputPath, outputPath, targetSizeMB)) {
        std::cerr << "Failed to compress video.\n";
        return 1;
    }

    std::cout << "Video compressed successfully to " << outputPath << std::endl;
    return 0;
}