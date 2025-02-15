#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 [--use-nvidia | --no-nvidia] [model_name]"
    echo "  --use-nvidia : Use the NVIDIA renderer (default)."
    echo "  --no-nvidia  : Do not use the NVIDIA renderer."
    echo "  model_name   : (Optional) Model name to pass as an argument."
    exit 1
}

# Default option: use NVIDIA renderer
USE_NVIDIA=true
MODEL_NAME=""

# Parse arguments
if [[ $# -gt 2 ]]; then
    usage
elif [[ $# -ge 1 ]]; then
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
    
    # If there's a second argument, treat it as MODEL_NAME
    if [[ $# -eq 2 ]]; then
        MODEL_NAME="$2"
    fi
fi

# Set environment variables based on the option
if $USE_NVIDIA; then
    echo "Using NVIDIA renderer..."
    CMD="MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA MESA_GL_VERSION_OVERRIDE=4.3 ./install/bin/glfw-imgui/glfw-imgui"
else
    echo "Using default renderer..."
    CMD="MESA_GL_VERSION_OVERRIDE=4.3 ./install/bin/glfw-imgui/glfw-imgui"
fi

# Append model name if provided
if [[ -n "$MODEL_NAME" ]]; then
    CMD+=" $MODEL_NAME"
fi

# Execute command
eval $CMD

