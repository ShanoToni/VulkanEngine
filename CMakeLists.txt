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
# Set GLFW options before adding it
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
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
include_directories(include/Renderer)
file(GLOB_RECURSE SOURCES src/*.cpp include/*.h include/*.hpp include/Shaders/*.cpp)
add_executable(VkEngine ${SOURCES})
target_include_directories(VkEngine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_include_directories(VkEngine PUBLIC ${GLM_INC})
target_include_directories(VkEngine PUBLIC ${RENDER_INC})

if(APPLE)
    target_link_libraries(VkEngine renderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan
        "-framework Cocoa"
        "-framework Metal"
        "-framework QuartzCore"
        "-framework IOKit"
    )
elseif(UNIX)
	target_link_libraries(VkEngine renderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)
endif()

# Compile the shaders based on platform
if(MSVC)
   execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/compile.bat)
elseif(APPLE)
	message("Compiling mac shaders")
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/compile_mac.sh)
elseif(UNIX)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/compile.sh)
endif()


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
	add_dependencies(VkEngine copy_resources)
elseif(APPLE)
	message("Copying resources to mac build dir")
	file(COPY "${CMAKE_SOURCE_DIR}/resources"
    	 DESTINATION "${CMAKE_BINARY_DIR}/bin")
else()
	# Copy
	message("Copying resources to build directory")
	add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND}
		-E copy_directory
		"${PROJECT_SOURCE_DIR}/resources"
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}resources
	)
	add_dependencies(VkEngine copy_resources)
endif()




