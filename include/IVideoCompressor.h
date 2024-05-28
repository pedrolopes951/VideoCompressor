#pragma once


#include <string>

/*
* Strategy Interface - common method, compress, that all concrete strategies must implement. Each strategy will provide its own implementation of how to compress video files.
*/

class IVideoCompressor {
public:
    virtual ~IVideoCompressor() {}
    virtual bool compress(const std::string& inputPath, const std::string& outputPath, int targetSizeMB) = 0;
    virtual bool compress(const std::string& directoryPath, int targetSizeMB) = 0;

};
