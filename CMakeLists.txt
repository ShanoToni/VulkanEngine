cmake_minimum_required(VERSION 3.10)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif(MSVC)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VulkanEngine)


SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Dependencies
# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
set(GLFW_INC ${CMAKE_SOURCE_DIR}/external/glfw/include)
link_directories(${CMAKE_SOURCE_DIR}/external/glfw/lib)


# GLM
set(GLM_INC ${CMAKE_SOURCE_DIR}/external/glm/glm)

# Vulkan
find_package(Vulkan REQUIRED)

# Renderer
add_subdirectory("Renderer")
set(RENDER_INC ${CMAKE_SOURCE_DIR}/Renderer/include)
link_directories("${CMAKE_SOURCE_DIR}/Renderer/include")

# Executable
include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp include/*.h include/*.hpp)
add_executable(VkEngine ${SOURCES})
target_include_directories(VkEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(VkEngine PUBLIC ${GLM_INC})
target_include_directories(VkEngine PUBLIC ${RENDER_INC})
target_link_libraries(VkEngine renderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)

# Compile the shaders based on platform
if(MSVC)
   execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/compile.bat)
endif(MSVC)

if(UNIX)
   execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/compile.sh)
endif(UNIX)

if (MSVC)
	set_target_properties(VkEngine PROPERTIES 
		VS_DEBUGGER_WORKING_DIRECTORY
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
	)

	# Copy
	add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
		-E copy_directory
		"${PROJECT_SOURCE_DIR}/resources"
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources
	)

else()
	# Copy
	add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
		-E copy_directory
		"${PROJECT_SOURCE_DIR}/resources"
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}resources
	)
endif()
add_dependencies(VkEngine copy_resources)



