@echo off
START glslc.exe ../resources/shaders/fragment.frag -o ../resources/shaders/frag.spv
START glslc.exe ../resources/shaders/vertex.vert -o ../resources/shaders/vert.spv