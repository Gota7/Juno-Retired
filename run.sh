export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
mkdir -p build
cd build
cmake ..
make -j8
cd ..
bin/Juno