#include <iostream>
extern "C" {
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

int main() {
    printVersionLibs();
    return 0;
}

