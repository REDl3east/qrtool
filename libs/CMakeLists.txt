include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG 8c9beb0c873f6ca5efbd88f1ad2648bfc793b2ac
)

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

FetchContent_MakeAvailable(SDL2)