cmake_minimum_required(VERSION 3.21)
project(vkt)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(./glfw-3.3.6)
file(COPY ./assets/config DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ./assets/gamedata DESTINATION ${CMAKE_BINARY_DIR})

add_executable(vkt
        headers/chunk.hpp
        headers/cmesh.hpp
        headers/config.hpp
        headers/crosshair.hpp
        headers/cursor.hpp
        headers/entity.hpp
        headers/globaldef.hpp
        headers/ivec.hpp
        headers/mesh.hpp
        headers/perlin.hpp
        headers/player.hpp
        headers/region.hpp
        headers/render.hpp
        headers/shader.hpp
        headers/stb_image.hpp
        headers/torch.hpp
        headers/veng.hpp
        headers/vhit.hpp
        headers/world.hpp
        src/chunk.cpp
        src/cmesh.cpp
        src/config.cpp
        src/crosshair.cpp
        src/cursor.cpp
        src/ivec.cpp
        src/mesh.cpp
        src/perlin.cpp
        src/player.cpp
        src/render.cpp
        src/shader.cpp
        src/torch.cpp
        src/veng.cpp
        src/world.cpp)

target_link_libraries(vkt glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(vkt OpenGL::GL)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
target_link_libraries(vkt GLEW::GLEW)