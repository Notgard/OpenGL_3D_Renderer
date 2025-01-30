#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 [--use-nvidia | --no-nvidia]"
    echo "  --use-nvidia : Use the NVIDIA renderer (default)."
    echo "  --no-nvidia  : Do not use the NVIDIA renderer."
    exit 1
}

# Default option: use NVIDIA renderer
USE_NVIDIA=true

# Parse arguments
if [[ $# -gt 1 ]]; then
    usage
elif [[ $# -eq 1 ]]; then
    case "$1" in
        --use-nvidia)
            USE_NVIDIA=true
            ;;
        --no-nvidia)
            USE_NVIDIA=false
            ;;
        *)
            usage
            ;;
    esac
fi

# Set environment variables based on the option
if $USE_NVIDIA; then
    echo "Using NVIDIA renderer..."
    MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA MESA_GL_VERSION_OVERRIDE=4.3 ./install/bin/glfw-imgui/glfw-imgui
else
    echo "Using default renderer..."
    MESA_GL_VERSION_OVERRIDE=4.3 ./install/bin/glfw-imgui/glfw-imgui
fi

