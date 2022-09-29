subs/tracy/profiler/build/unix/Tracy-release &
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
mkdir -p build
cd build
cmake .. -DTRACY_ENABLE=1
make -j8
cd ..
bin/Juno