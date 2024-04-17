#include "../include/UtilsCompression.h"

namespace Utils
{

    // Function to execute a shell command and capture the output.
    std::string exec(const char *cmd)
    {
        std::array<char, 128> buffer; // Buffer to read the output
        std::string result;
        // Pipe to send the std out of the process opened by the popen if the duration of the video.decltype(&pclose) is a explicit deleter function to handle the popen
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose); // Safely manages the file pointer that interacts with the pipe "r"
        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        { // Get the data from the pipe into the buffer
            result += buffer.data();
        }
        return result;
    }
    // Function to get the duration of the video in seconds
    double getVideoDuration(const std::string &inputPath)
    {
        std::string cmd = "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"" + inputPath + "\""; // ffprobe to fetch the duration of the video
        
        std::string output = exec(cmd.c_str());
        return std::stod(output);
        //return 0.0;
    }
    /// @brief function calculates how many bits per second (bps) you can use for the video part, aiming to meet a desired file size (in MB) over a given duration (in seconds)
    /// @param fileSizeMB Desired file size of the video to compress
    /// @param durationSeconds Duration of the original video
    /// @return return the adjusted bit rate for the video compressed
     int calculateTargetBitrate(int targetSizeMB,int videoDuration)
    {
        if (videoDuration == 0)
            return 0;                                     // Prevent division by zero
        int totalBits = targetSizeMB * 8192;                // 8 bits in a byte, 1024 bytes in a kilobyte
        int audioBits = 128 * videoDuration;            // Assuming 128 kbps audio bitrate
        return (totalBits - audioBits) / videoDuration; // Adjust bitrate for video only
    }

}