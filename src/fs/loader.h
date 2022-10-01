#pragma once

#include "path.h"
#include <fstream>
#include <string>

namespace FLoader
{

    // Load an ifstream. No need to feed it a relative path. Remember to close it.
    inline std::ifstream OpenFile(const std::string& path)
    {
        return std::ifstream(FPath::RelPath(path));
    }

    // Load an ofstream. No need to feed it a relative path. Remember to close it.
    inline std::ofstream SaveFile(const std::string& path)
    {
        return std::ofstream(FPath::RelPath(path));
    }

    // Load all text from a file. No need to feed it a relative path.
    inline std::string ReadAllLines(const std::string& path)
    {
        auto ifs = OpenFile(path);
        std::string ret = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        ifs.close();
        return ret;
    }

}