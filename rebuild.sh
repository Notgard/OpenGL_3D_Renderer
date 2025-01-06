#!/bin/bash

# Define paths
BUILD_DIR="build"
INSTALL_DIR="../install"
CMAKE_OPTIONS="-G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DUSING_CONAN=0 -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake .."

# Function to clean build directory
clean_build() {
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"/*
}

# Check if clean build is requested
read -p "Do you want to perform a clean build? (y/n): " clean_choice

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Navigate to the build directory
cd "$BUILD_DIR"

if [[ "$clean_choice" == "y" || "$clean_choice" == "Y" ]]; then
    clean_build
fi

# Re-run cmake configuration
echo "Configuring with cmake..."
cmake $CMAKE_OPTIONS

# Build and install
echo "Building and installing..."
cmake --build . --target install

echo "Build completed successfully!"

