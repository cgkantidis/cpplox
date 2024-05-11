if [ $# -eq 0 ]; then
  BUILD_TYPE=Release
else
  BUILD_TYPE="$1"
fi
rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold" && \
cmake --build build -j$(nproc) && \
ctest --test-dir build/tests --output-on-failure
