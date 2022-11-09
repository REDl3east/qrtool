cmake_minimum_required(VERSION 3.0.0)
project(sdl2-qr-code VERSION 0.1.0)

set(SDL2_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/SDL2)
set(SDL2_INCLUDE_DIR ${SDL2_SORUCE_DIR}/include)

set(QR_CODE_GEN_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/qr_code_gen-src/c)
set(QR_CODE_GEN_INCLUDE_DIR ${QR_CODE_GEN_SORUCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(sdl2-qr-code main.c)
target_link_libraries(sdl2-qr-code SDL2-static qr_code_gen)
target_include_directories(sdl2-qr-code PUBLIC ${SDL2_INCLUDE_DIR} ${QR_CODE_GEN_INCLUDE_DIR})