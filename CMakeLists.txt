cmake_minimum_required(VERSION 3.11)
project(Juno)
include_directories(include)

if(APPLE)
    include_directories(/usr/local/include)
endif()

set(CMAKE_C_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS "-O3 -fpermissive")

set(BINARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})

include_directories("src")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.c")

# Libs.
add_subdirectory(subs/glfw)
include_directories(subs/glfw/include)
add_subdirectory(subs/assimp)
include_directories(subs/glfw/assimp/include)
add_subdirectory(subs/yaml-cpp)
include_directories(subs/glfw/yaml-cpp/include)
add_subdirectory(subs/pugixml)
include_directories(subs/pugixml)
include_directories(subs/glm/glm)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES} assimp yaml-cpp pugixml X11 pthread Xrandr Xi dl)
target_include_directories(${PROJECT_NAME} PUBLIC include)