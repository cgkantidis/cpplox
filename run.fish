#!/usr/bin/env fish

if test (count $argv) -eq 0
  set BUILD_TYPE "Release"
else
  set BUILD_TYPE "$argv[1]"
end

rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold" && \
cmake --build build -j$(nproc) && \
ctest --test-dir build/tests --output-on-failure
