mkdir -p build
cd build
conan install .. --update --build=missing --remote=conancenter
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release\
   -DCMAKE_INSTALL_PREFIX="../install"\
   -DUSING_PACKAGE_MANAGER_CONAN=1\
   -DCMAKE_PREFIX_PATH="./Release/generators"\
   -DCMAKE_TOOLCHAIN_FILE=./Release/generators/conan_toolchain.cmake\
   ..
cmake --build . --target install
