cmake_minimum_required(VERSION 3.10)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif(MSVC)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(VulkanRenderer)

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Executable
include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp include/*.h include/*.hpp)
add_library(renderer STATIC ${SOURCES})

target_include_directories(renderer PRIVATE ${GLFW_INC})
target_include_directories(renderer PUBLIC ${GLFW_INC})
target_link_libraries(renderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)

# Dependencies
# GLFW
set(GLFW_INC ${CMAKE_SOURCE_DIR}/external/glfw/include)
link_directories("${CMAKE_SOURCE_DIR}/external/glfw/lib")

# GLM
set(GLM_INC ${CMAKE_SOURCE_DIR}/external/glm/glm)

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(renderer PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(renderer Vulkan::Vulkan)

# Build Dependencies
#add_executable(ENGINE ${SOURCES})
include_directories(renderer PRIVATE ${SFML_INCS} ${GLM_INC})
link_libraries(renderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)
