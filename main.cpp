#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>

// Going in this blindly
// making a file tree viewer
// No AI

#ifdef _WIN32
#include <windows.h>
#endif

int16_t directory_nesting_count{};

/*

|___

*/
std::string repeat_string(std::string_view string, int16_t count)
{
    std::string result;

    result.reserve(string.size() * count);

    for (int16_t i{}; i < count; ++i)
        result += string;

    return result;
}

std::string get_indentation_styling()
{
    std::string indent_styling = "\u2502" + std::string(3, ' ') ;

    return repeat_string(indent_styling, directory_nesting_count) + "\u251C ";
}

void crawl_directory(const std::filesystem::path& path)  
{
    for (const auto& entry : std::filesystem::directory_iterator{path})
    {
        if (entry.is_directory())
        {
            std::cout << get_indentation_styling()
                      << entry.path().filename().string() 
                      << std::endl;

            directory_nesting_count++;

            crawl_directory(entry.path());

            directory_nesting_count--;
        }
        else
        {
            std::cout << get_indentation_styling()
                      << entry.path().filename().string() 
                      << std::endl;
        }
    }
}

int main(int argc, char* argv[]) 
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // default path
    std::filesystem::path path {"TestFolder"};

    if (argc > 1) { path = argv[1]; }

    crawl_directory(path); 

    return 0;
}
