// Going in this blindly
// making a file tree viewer
// No AI 

/// TODO: Support hiding . files. Should be the default, then second arg of 'all' displays everything.

/// TODO: (huge) Implement file traversal within terminal. 
///   Should use nvim movement keys, 'l' to open directory, 'j' to go back one.
///   Activated by some second arg, no recursion here, each termninal refresh is just a loop through current dir level.
///   Opening directories is just a refresh with dir_iterator of the selected path.
///    Perhaps a map is good for this? 
///       j & k keys (up and down) (in)(de)crements a counter, l looks up the key of an items path from our map using the counter variable.

//  ftc show "path"
//  ftc showall "path" (show hidden files)
//  ftc traverse
//  ftc help

#include <iostream>
#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>

// Allows us to properly display Unicode symbols on windows.
#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

int16_t directory_nesting_count{};
bool is_first_directory = true;
bool show_hidden_dirs = false;

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
        if (!show_hidden_dirs && entry.path().filename().string().starts_with('.')) continue;

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

    if (argc < 2) { std::cout  << "No arguments passed. Run <[appname] help> for command list"; return 1;};

    const std::vector<std::string_view> args(argv, argv + argc);

    /// TODO:: Handle not passing in second argument after <show> or <showall>
    if (args[1] == "show")
    {
        fs::path path {args[2]};

        if (!std::filesystem::exists(path))
        {
            std::cerr << "Cannot open directory " << path << ": No such file or directory" << std::endl; 
            return 1;
        }

        crawl_directory(path);
    }
    else if (args[1] == "showall")
    {
        show_hidden_dirs = true;

        fs::path path {args[2]};

        if (!std::filesystem::exists(path))
        {
            std::cerr << "Cannot open directory " << path << ": No such file or directory" << std::endl; 
            return 1;
        }

        crawl_directory(path);
    }
    else if (args[1] == "traverse")
    {
        std::cout << "coming soon!!!!" << std::endl;

        // void traverse_directory(const fs::path& from_path)
    }
    else if (args[1] == "help")
    {
        std::cout << "[appname] show - Show all directories and sub-directories \n"
                  << "[appname] showall - Show all directories and sub-directories, hidden dirs included \n"
                  << "[appname] traverse - Show and traverse directories"
                  << std::endl;
    }
    else 
    {
        std::cerr << "Command of '" << args[1] << "' not recognized. Run <[appname] help> for command list" << std::endl;

        return 1;
    }

    return 0;
}
