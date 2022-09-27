#include "loader.h"

#include "path.h"

inline std::ifstream FLoader::OpenFile(const std::string& path)
{
    return std::ifstream(FPath::RelPath(path));
}

inline std::ofstream FLoader::SaveFile(const std::string& path)
{
    return std::ofstream(FPath::RelPath(path));
}

inline std::string FLoader::ReadAllLines(const std::string& path)
{
    auto ifs = OpenFile(path);
    return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();
}