mkdir -p build
cd build
cmake .. -DTRACY_ENABLE=0 -DCROSS_COMPILE_MAC=true -D CMAKE_TOOLCHAIN_FILE=toolchains/mac.cmake
make -j12
cd ..