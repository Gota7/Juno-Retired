#pragma once

#include <string>

namespace FPath {

    // Get the path of the executable.
    std::string GetExecutablePath();

    // Get the directory that the executable is in.
    std::string GetExecutableDir();

    // Join two paths together.
    std::string MergePaths(std::string pathA, std::string pathB);

    // Check if a given path exists.
    bool FileExists(const std::string& filePath);

    // Convert a path to a relative one.
    std::string RelPath(std::string path);

}