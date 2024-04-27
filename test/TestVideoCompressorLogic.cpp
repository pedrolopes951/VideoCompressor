#include <cstdio>
#include <iostream>

int main() {
    // Command to list files in the current directory
    const char* cmd = "ls";

    // Open a pipe to execute the command
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
        return EXIT_FAILURE;
    }

    // Read and print the output of the command
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;
    }

    // Close the pipe
    pclose(pipe);

    return EXIT_SUCCESS;
}
