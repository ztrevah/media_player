// main.cpp
#include "Application.h"

int main(int argc, char* argv[]) {
    std::string startDirectory = "."; // Default to current directory

    if (argc > 1) {
        startDirectory = argv[1]; // Get directory from command line argument
    }

    Application app(startDirectory); // Create Application instance
    app.start(); // Start the application

    return 0;
}