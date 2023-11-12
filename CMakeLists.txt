cmake_minimum_required(VERSION 3.20)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
set(glm_DIR glm/cmake/glmConfig.cmake.in)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(glfw/include)
include_directories(glad/include)

add_executable(lab1 main.cpp glad/src/glad.c)

target_link_libraries(lab1 glfw)
target_link_libraries(lab1 OpenGL::GL)