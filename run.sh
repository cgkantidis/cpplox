rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=ASAN
cmake --build build -j$(nproc)
ctest --test-dir build/tests --output-on-failure
