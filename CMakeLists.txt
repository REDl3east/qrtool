cmake_minimum_required(VERSION 3.0.0)
project(sdl2-qr-code VERSION 0.1.0)

set(QR_CODE_GEN_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/qr_code_gen-src/c)
set(QR_CODE_GEN_INCLUDE_DIR ${QR_CODE_GEN_SORUCE_DIR})

set(ARGTABLE3_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/argtable3-src/src)
set(ARGTABLE3_INCLUDE_DIR ${ARGTABLE3_SORUCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(sdl2-qr-code main.c)
target_link_libraries(sdl2-qr-code SDL2-static SDL2_image qr_code_gen argtable3)
target_include_directories(sdl2-qr-code PUBLIC ${QR_CODE_GEN_INCLUDE_DIR} ${ARGTABLE3_INCLUDE_DIR})