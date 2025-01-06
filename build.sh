cd build
conan install .. --update --build=missing --remote=conancenter
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release\
   -DCMAKE_INSTALL_PREFIX="../install"\
   -DUSING_PACKAGE_MANAGER_CONAN=1\
   -DCMAKE_PREFIX_PATH="./build/Release/generators"\
   -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake\
   ..
cmake --build . --target install
