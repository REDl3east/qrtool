#include <stdio.h>

#include "SDL.h"
#include "qrcodegen.h"

static void printQr(const uint8_t qrcode[]) {
  int size   = qrcodegen_getSize(qrcode);
  int border = 4;
  for (int y = -border; y < size + border; y++) {
    for (int x = -border; x < size + border; x++) {
      fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
    }
    fputs("\n", stdout);
  }
  fputs("\n", stdout);
}

static const char* APP_NAME = "SDL2 QR Code";
static int INITIAL_WIDTH    = 1280;
static int INITIAL_HEIGHT   = 720;

int main(int argv, char** argc) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return 1;
  }

  Uint32 flags       = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
  SDL_Window* window = SDL_CreateWindow(APP_NAME, INITIAL_WIDTH * 0.25, INITIAL_HEIGHT * 0.25, INITIAL_WIDTH, INITIAL_HEIGHT, flags);

  if (!window) {
    printf("[ERROR] SDL_CreateWindow: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    printf("[ERROR] SDL_CreateRenderer: %s\n", SDL_GetError());
    return 1;
  }

  if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
    printf("[ERROR] SDL_SetRenderDrawBlendMode: %s\n", SDL_GetError());
    return 1;
  }

  const char* text             = "WIFI:S:Aspen;T:WPA;P:Overmyer!5;;";
  enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_HIGH;   

  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
  bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (!ok) {
    printf("[ERROR] failed to generate QR code.\n");
    return 1;
  }

  int qr_size          = qrcodegen_getSize(qrcode);
  SDL_Surface* surface = SDL_CreateRGBSurface(0, qr_size, qr_size, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  if (surface == NULL) {
    printf("[ERROR] fSDL_CreateRGBSurface: %s\n", SDL_GetError());
    return 1;
  }

  for (int x = 0; x < qr_size; x++) {
    for (int y = 0; y < qr_size; y++) {
      SDL_Rect r1 = {x, y, 1, 1};
      if (qrcodegen_getModule(qrcode, x, y)) {
        SDL_FillRect(surface, &r1, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
      } else {
        SDL_FillRect(surface, &r1, SDL_MapRGBA(surface->format, 255, 255, 255, 255));
      }
    }
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == NULL) {
    printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Point size;
  SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
  printf("generated QR code: %dx%d\n", size.x, size.y);

  float scale = (INITIAL_HEIGHT - 1) / qr_size;

  int done = 0;

  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if ((event.type == SDL_QUIT) ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) ||
          (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))) {
        done = 1;
      }
      switch (event.type) {
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_1) {
            printf("%d\n", 1);
            scale -= 0.1;
          } else if (event.key.keysym.sym == SDLK_2) {
            printf("%d\n", 2);
            scale += 0.1;
          } else if (event.key.keysym.sym == SDLK_3) {
            printf("%d\n", 3);
          }

          break;
        }
        case SDL_MOUSEWHEEL: {
          if (event.wheel.y > 0) {
            scale += 0.1;
          } else if (event.wheel.y < 0) {
            scale -= 0.1;
          }

          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0xe2, 0x7d, 0x60, 255);
    SDL_RenderClear(renderer);

    SDL_Rect r2 = {0.5 * (INITIAL_WIDTH - 512), 0.5 * (INITIAL_HEIGHT - 512), 512, 512};

    SDL_RenderCopy(renderer, texture, NULL, &r2);

    SDL_RenderPresent(renderer);
  }

  SDL_FreeSurface(surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
