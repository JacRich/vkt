cmake_minimum_required(VERSION 3.21)
project(vkt)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)

file(COPY ./assets/config DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ./assets/gamedata DESTINATION ${CMAKE_BINARY_DIR})

add_executable(vkt
        headers/chunk.h
        headers/cmesh.h
        headers/config.h
        headers/cursor.h
        headers/entity.h
        headers/globaldef.h
        headers/ivec.h
        headers/mesh.h
        headers/perlin.h
        headers/player.h
        headers/region.h
        headers/render.h
        headers/shader.h
        headers/stb_image.h
        headers/veng.h
        headers/vhit.h
        headers/world.h
        src/chunk.cpp
        src/cmesh.cpp
        src/config.cpp
        src/cursor.cpp
        src/ivec.cpp
        src/mesh.cpp
        src/perlin.cpp
        src/player.cpp
        src/render.cpp
        src/shader.cpp
        src/veng.cpp
        src/world.cpp)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(vkt glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(vkt OpenGL::GL)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
target_link_libraries(vkt GLEW::GLEW)