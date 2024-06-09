#!/usr/bin/env zsh
git clone --depth=1 --branch llvmorg-17.0.6 https://github.com/llvm/llvm-project
cd llvm-project
cmake -B build -S runtimes \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DLLVM_USE_SANITIZER=MemoryWithOrigins
cmake --build build -j$(nproc) -- cxx cxxabi
