# Video Compressor

## Overview
Tool for compressing video files. It uses the Two-Pass compression method to optimize video size reduction while maintaining quality. The backend is powered by FFmpeg for video processing, SDL2 for handling graphical output [under development], and Dear ImGui for a  GUI implementation [under development].

## Dependencies
- **FFmpeg 1.0**: For video processing.
- **SDL2**: For rendering graphical components.
- **Dear ImGui**: For an interactive graphical user interface.
- These libraries should be installed in the `/c/` directory on your system.

## Installation

### Prerequisites
Ensure you have the following installed on your system:
- FFmpeg 1.0
- SDL2
- Dear ImGui
These should be located in the `/c/` directory as the project setup relies on this path for linking.

### Compiling the Project
To compile the project, use the following steps. Make sure you are using MSYS or MinGW64 terminal:

1. Navigate to the project directory.
2. Run the makefile provided with the command:
   ```bash
   make
    ```
### Running the Program

Once the project is compiled, you can run the program using the following command format:
```bash
    ./build/VideoCompressor <input_video_path> <output_video_path> <target_size_MB>
```
- <input_video_path>: Path to the input video file.
- <output_video_path>: Path where the compressed video will be saved.
- <target_size_MB>: Desired size of the output video in megabytes.

## GUI
The GUI, built with SDL2 and Dear ImGui, is under development. It aims to provide an easy-to-use interface for video compression settings, allowing users to choose compression methods, set file paths, and specify target sizes without command-line arguments.

