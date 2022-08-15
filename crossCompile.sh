mkdir -p build
cd build
cmake .. -DCROSS_COMPILE_WIN=true -D CMAKE_TOOLCHAIN_FILE=subs/glfw/CMake/x86_64-w64-mingw32.cmake
make -j12
cd ..