@echo off
setlocal
	glslc.exe --target-env=vulkan1.2 ../resources/shaders/fragment.frag -o ../resources/shaders/frag.spv 
	glslc.exe --target-env=vulkan1.2 ../resources/shaders/vertex.vert -o ../resources/shaders/vert.spv
endlocal
pause