#include <iostream>
#include <string>
#include <filesystem>

// Going in this blindly
// making a file tree viewer
// No AI

int main(int argc, char* argv[]) 
{
    std::string path = "."; 

    if (argc > 1) { path = argv[1]; }

    for (const auto& entry : std::filesystem::directory_iterator{path})
    {
        if (entry.path().filename().string()[0] == '.') { continue; }

        std::cout << entry.path().filename() << std::endl;
    }

    return 0;
}
