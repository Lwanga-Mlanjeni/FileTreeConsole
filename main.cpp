#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>

// Going in this blindly
// making a file tree viewer
// No AI

/*

    About <filesystem>

    std::filesystem::diretory_entry - represents a directory entry, contains `path` as memeber.
        is_directory()
        is_regular_file()
        
    std::filesystem::path - represents the filepath
        root_path()
        filename()
        root_directory()
        extension()

        also contains "has_something()" member functions that check if the corresponding path element is empty
            e.g. has_root_directory()
*/

void crawl_directory(const std::filesystem::path& path)  
{
    for (const auto& entry : std::filesystem::directory_iterator{path})
    {
        if (entry.is_directory())
        {
            std::cout << "directory: " << entry.path().filename().string() << std::endl;

            crawl_directory(entry.path());
        }
        else
        {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) 
{
    // default path
    std::filesystem::path path {"TestFolder"};

    if (argc > 1) { path = argv[1]; }

    crawl_directory(path); 

    return 0;
}
