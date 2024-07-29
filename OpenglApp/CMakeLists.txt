cmake_minimum_required(VERSION 3.10)

project(OpenGLApp)

set(CMAKE_CXX_STANDARD 11)

# Add source files
set(SOURCES main.cpp)

# Add include directories
include_directories(include)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLM (GL Mathematics)
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
include_directories(${GLM_INCLUDE_DIR})

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

# Add executable
add_executable(OpenGLApp ${SOURCES})

# Link libraries
target_link_libraries(OpenGLApp ${OPENGL_LIBRARIES} GLEW::GLEW glfw)
