./glslc --target-env=vulkan1.2 ../resources/shaders/vertex.vert -o vert.spv
./glslc --target-env=vulkan1.2 ../resources/shaders/fragment.frag -o frag.spv

./glslc --target-env=vulkan1.2 ../resources/shaders/physics.vert -o physics_vert.spv
./glslc --target-env=vulkan1.2 ../resources/shaders/physics.frag -o physics_frag.spv