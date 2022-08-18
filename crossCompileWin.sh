mkdir -p build
cd build
cmake .. -DCROSS_COMPILE_WIN=true -D CMAKE_TOOLCHAIN_FILE=toolchains/windows.cmake
make -j12
cd ..