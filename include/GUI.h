#pragma once

#include "../include/VideoCompressorContext.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <string>


/*
* Front-Facing interface for all GUI. 
*/

enum class CompressionMethod {
    TwoPass,   
    //TODO: Need to had more methods here
};

extern const char* CompressionMethodNames[]; // Declaration

// Map enum to string for display in GUI, if necessary

class GUI {
public:
    GUI();
    ~GUI();
    void initialize(); // Setup SDL and ImGui
    void render(); // Main render loop
    void cleanup(); // Cleanup resources
private:
    VideoCompressorContext* compressorContext;
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    char inputFilePath[1024];   // Buffer for input file path
    char outputPath[1024];      // Buffer for output file path
    int targetSizeMB;
    CompressionMethod selectedMethod;
    void setupImGuiStyle();
    void showSettingsWindow(); // New method to show settings
};