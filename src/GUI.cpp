#include "../include/GUI.h"
#include "../include/VideoCompressor.h"
#include <iostream>

const char *g_CompressionMethodNames[] = {"Two-Pass"};
const int g_NumberMethods = 1;


GUI::GUI() :selectedMethod(CompressionMethod::TwoPass), targetSizeMB{3}
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

    // Memset Input File and Output File to no string
    window = SDL_CreateWindow("Video Compressor",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
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

      // INitialize the bufers of the input and output with example paths
    strcpy(inputFilePath,"C:\\VideoCompressor\\test\\TestVideos");
    strcpy(outputPath,"C:\\VideoCompressor\\test\\TestVideos\\outputVideo.mp4");

    compressorContext = new VideoCompressorContext(nullptr);
}

void GUI::showSettingsWindow()
{
    ImGui::Begin("Compression Settings");

    static bool compressedDirectory = true;
    ImGui::Checkbox("Compressed entire directory", &compressedDirectory);

    // Input text for file path with placeholder text
    if (compressedDirectory) {
        ImGui::InputText("Input Directory",inputFilePath, sizeof(inputFilePath));
    } else {
        ImGui::InputText("Input Path",inputFilePath, sizeof(inputFilePath));
        ImGui::InputText("Output Path", outputPath, sizeof(outputPath));
    }

    // Dropdown for selecting compression method
    if (ImGui::BeginCombo("Compression Method", g_CompressionMethodNames[static_cast<int>(selectedMethod)]))
    {
        for (int i = 0; i < g_NumberMethods; i++)
        {
            bool isSelected = (selectedMethod == static_cast<CompressionMethod>(i));
            if (ImGui::Selectable(g_CompressionMethodNames[i], isSelected))
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
        // Handle compression based on the toggle state
        if (compressedDirectory) {
            // Assume compressDirectory handles directory compression
            compressorContext->compressVideo(inputFilePath, targetSizeMB);
        } else {
            // Standard file compression
            compressorContext->compressVideo(inputFilePath, outputPath, targetSizeMB);
        }
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