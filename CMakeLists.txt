cmake_minimum_required(VERSION 3.0.0)
project(qrtool VERSION 0.1.0)

set(QR_CODE_GEN_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/qr_code_gen-src/c)
set(QR_CODE_GEN_INCLUDE_DIR ${QR_CODE_GEN_SORUCE_DIR})

set(ARGTABLE3_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/argtable3-src/src)
set(ARGTABLE3_INCLUDE_DIR ${ARGTABLE3_SORUCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(qrtool main.c)
target_link_libraries(qrtool SDL2-static SDL2_image qr_code_gen argtable3)
target_include_directories(qrtool PUBLIC ${QR_CODE_GEN_INCLUDE_DIR} ${ARGTABLE3_INCLUDE_DIR})

install(TARGETS qrtool DESTINATION /usr/bin/)