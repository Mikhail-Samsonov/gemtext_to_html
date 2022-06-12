# gemtext_to_html

# build

  rm -rf build

  mkdir -p build && cd build

  conan install .. -scompiler=gcc -scompiler.version=9 -scompiler.libcxx=libstdc++11 --build=missing

  cmake -DCMAKE_MODULE_PATH="$(pwd)" -DCMAKE_BUILD_TYPE=Debug ..

  cmake --build .
