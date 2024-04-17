#include "../include/GUI.h"
#include "../include/VideoCompressor.h"
#include <iostream>

const char *CompressionMethodNames[] = {"Two-Pass", "Single-Pass"};

GUI::GUI()
{
}

GUI::~GUI()
{
    cleanup();
    delete compressorContext;
}

void GUI::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Window *window = SDL_CreateWindow("Video Compressor",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    setupImGuiStyle();
}

void GUI::showSettingsWindow()
{
    ImGui::Begin("Compression Settings");

    // Input text for file path
    ImGui::InputText("Input File Path", inputFilePath, sizeof(inputFilePath));
    ImGui::InputText("Output File Path", outputPath, sizeof(inputFilePath));
    // Dropdown for selecting compression method
    if (ImGui::BeginCombo("Compression Method", CompressionMethodNames[static_cast<int>(selectedMethod)]))
    {
        for (int i = 0; i < 2; i++)
        {
            bool isSelected = (selectedMethod == static_cast<CompressionMethod>(i));
            if (ImGui::Selectable(CompressionMethodNames[i], isSelected))
            {
                selectedMethod = static_cast<CompressionMethod>(i);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Input integer for target size
    ImGui::InputInt("Target Size (MB)", &targetSizeMB);

    if (ImGui::Button("Compress"))
    {

        switch (selectedMethod)
        {
        case CompressionMethod::TwoPass:
            compressorContext->setCompressor(new TwoPassCompressor()); // Method to update the strategy based on selection
            break;
        default:
            std::cerr << "Not valid compresion method, try again" << std::endl;
            break;
        }
        compressorContext->compressVideo(inputFilePath, outputPath, targetSizeMB);
    }

    ImGui::End();
}

void GUI::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color
    SDL_RenderClear(renderer);                      // Clear the screen with the background color

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    showSettingsWindow();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(renderer); // Update screen
}

void GUI::cleanup()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(window);     
    SDL_Quit();
}

void GUI::setupImGuiStyle()
{
    ImGui::StyleColorsDark(); 
}