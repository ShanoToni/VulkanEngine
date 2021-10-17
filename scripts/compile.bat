@echo off
setlocal
	glslc.exe ../resources/shaders/fragment.frag -o ../resources/shaders/frag.spv 
	glslc.exe ../resources/shaders/vertex.vert -o ../resources/shaders/vert.spv
endlocal
pause