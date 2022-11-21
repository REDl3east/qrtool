include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG 8c9beb0c873f6ca5efbd88f1ad2648bfc793b2ac
)

FetchContent_Declare(
  SDL2_IMAGE
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG a861543ab78f5d5e18e6af43964ce8df60cb30b3
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  argtable3
  GIT_REPOSITORY https://github.com/argtable/argtable3.git
  GIT_TAG f25c624e3a5a65f4d1897aa7d51c897f8f8098d0
)

FetchContent_MakeAvailable(SDL2 SDL2_IMAGE argtable3)

FetchContent_Declare(
  qr_code_gen
  GIT_REPOSITORY https://github.com/nayuki/QR-Code-generator.git
  GIT_TAG 720f62bddb7226106071d4728c292cb1df519ceb
)

FetchContent_GetProperties(qr_code_gen)
if(NOT qr_code_gen_POPULATED)
  FetchContent_Populate(qr_code_gen)

  set(QR_CODE_SRC
    ${qr_code_gen_SOURCE_DIR}/c/qrcodegen.c
  )

  add_library(qr_code_gen STATIC ${QR_CODE_SRC})

endif()

