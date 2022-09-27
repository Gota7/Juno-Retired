#pragma once

#include <fstream>
#include <string>

namespace FLoader
{
    
    // Load an ifstream. No need to feed it a relative path. Remember to close it.
    inline std::ifstream OpenFile(const std::string& path);

    // Load an ofstream. No need to feed it a relative path. Remember to close it.
    inline std::ofstream SaveFile(const std::string& path);

    // Load all text from a file. No need to feed it a relative path.
    inline std::string ReadAllLines(const std::string& path);

}