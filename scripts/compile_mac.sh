#!/bin/bash

# Paths to shader source files
VERT_SHADER_PATH="../resources/shaders"
FRAG_SHADER_PATH="../resources/shaders"

PHYS_VERT_PATH="../resources/shaders"
PHYS_FRAG_PATH="../resources/shaders"

# Compile vertex shader
glslc "$VERT_SHADER_PATH/vertex.vert" -o $VERT_SHADER_PATH/vert.spv && echo "Compiled vertex shader."
glslc "$PHYS_VERT_PATH/physics.vert" -o $PHYS_VERT_PATH/physics_vert.spv && echo "Compiled physics vertex shader."

# Compile fragment shader
glslc "$FRAG_SHADER_PATH/fragment.frag" -o $FRAG_SHADER_PATH/frag.spv && echo "Compiled fragment shader."
glslc "$PHYS_FRAG_PATH/physics.frag" -o $PHYS_FRAG_PATH/physics_frag.spv && echo "Compiled physics fragment shader."

echo "Done."
