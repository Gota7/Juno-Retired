cmake_minimum_required(VERSION 3.11)
project(Juno)
include_directories(include)

if(APPLE)
    include_directories(/usr/local/include)
    set(STATIC_BUILD true)
endif()

if (WIN32)
    set(STATIC_BUILD true)
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
if (STATIC_BUILD)
    option(BUILD_SHARED_LIBS "" OFF)
    option(ASSIMP_WARNINGS_AS_ERRORS "" OFF)
    option(ASSIMP_INSTALL "" OFF)
    add_subdirectory(subs/glfw)
    include_directories(subs/glfw/include)
    add_subdirectory(subs/assimp)
    include_directories(subs/glfw/assimp/include)
    add_subdirectory(subs/yaml-cpp)
    include_directories(subs/glfw/yaml-cpp/include)
    add_subdirectory(subs/pugixml)
    include_directories(subs/pugixml)
    add_subdirectory(subs/glm)
    include_directories(subs/glm/glm)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
if (STATIC_BUILD)
    if (APPLE)
        target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES} assimp yaml-cpp pugixml pthread Xrandr Xi dl)
    elseif(WIN32)
        target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES} assimp yaml-cpp pugixml)
    else()
        target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES} assimp yaml-cpp pugixml X11 pthread Xrandr Xi dl)
    endif()
else()
    target_link_libraries(${PROJECT_NAME} glfw GL assimp yaml-cpp X11 pthread Xrandr Xi dl)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Resources.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)