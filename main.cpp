// Going in this blindly
// making a file tree viewer
// No AI 

/// TODO: Add error handling.
/// TODO: Support hiding . files. Should be the default, then second arg of 'all' displays everything.

/// TODO: (huge) Implement file traversal within terminal. 
///   Should use nvim movement keys, 'l' to open directory, 'j' to go back one.
///   Activated by some second arg, no recursion here, each termninal refresh is just a loop through current dir level.
///   Opening directories is just a refresh with dir_iterator of the selected path.
///    Perhaps a map is good for this? 
///       j & k keys (up and down) (in)(de)crements a counter, l looks up the key of an items path from our map using the counter variable.

#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

int16_t directory_nesting_count{};

bool is_first_directory = true;

// for styling
int16_t total_nesting_count{};

std::string repeat_string(std::string_view string, int16_t count)
{
    std::string result;

    result.reserve(string.size() * count);

    for (int16_t i{}; i < count; ++i)
        result += string;

    return result;
}

std::string get_indentation_styling(const std::string& dir_marker_symbol)
{
    std::string indent_styling = "\u2502" + std::string(3, ' ');

    return repeat_string(indent_styling, directory_nesting_count) + dir_marker_symbol;
}

void crawl_directory(const fs::path& path)  
{
    for (const auto& entry : fs::directory_iterator{path})
    {
        if (entry.is_directory())
        {
            std::cout << (is_first_directory ? get_indentation_styling("\u256D\u2500\u25A0 ") : get_indentation_styling("\u251C\u2500\u25A0 "))
                      << entry.path().filename().string() 
                      << std::endl;

            directory_nesting_count++;
            total_nesting_count++;

            crawl_directory(entry.path());

            directory_nesting_count--;
        }
        else
        {
            std::cout << (is_first_directory ? get_indentation_styling("\u256D\u2500\u25CB ") : get_indentation_styling("\u251C\u2500\u25CB "))
                      << entry.path().filename().string() 
                      << std::endl;
        }

        is_first_directory = false;
    }
}

int main(int argc, char* argv[]) 
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // default path
    fs::path path {"TestFolder"};

    if (argc > 1) { path = argv[1]; }

    crawl_directory(path);

    return 0;
}
