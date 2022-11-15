#include <stdio.h>

#include "SDL.h"
#include "qrcodegen.h"

#include "SDL_image.h"

static const char* APP_NAME = "SDL2 QR Code";
static int INITIAL_WIDTH    = 1280;
static int INITIAL_HEIGHT   = 720;

typedef struct QrAttr {
  char* input;
  enum qrcodegen_Ecc level;
  int boost_ecc;
  enum qrcodegen_Mask mask;
  int version_max;
  int version_min;
} QrAttr;

typedef struct QrData {
  uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
  uint8_t size;
  QrAttr attr;
} QrData;

typedef struct QrSurfaceAttr {
  SDL_Color foreground;
  SDL_Color background;
  int size;
} QrSurfaceAttr;

typedef struct QrSurface {
  SDL_Surface* surface;
  QrData qr;
  QrSurfaceAttr attr;
} QrSurface;

int createQrCode(QrData* qr);
int createQrCodeSurface(QrSurface* qr_surface);
int createQrCodeSurfaceScale(QrSurface* qr_surface, float scale);
int createQrCodeSurfaceWithSize(QrSurface* qr_surface, int size);
void destroyQrCodeSurface(QrSurface* qr_surface);

int main(int argv, char** argc) {
  QrSurface qr_surface;
  qr_surface.qr.attr.input       = "What are you looking at?";
  qr_surface.qr.attr.level       = qrcodegen_Ecc_HIGH;
  qr_surface.qr.attr.mask        = qrcodegen_Mask_AUTO;
  qr_surface.qr.attr.version_min = 1;
  qr_surface.qr.attr.version_max = 40;
  qr_surface.qr.attr.boost_ecc   = 1;

  qr_surface.attr.background = (SDL_Color){255, 255, 255, 255};
  qr_surface.attr.foreground = (SDL_Color){0, 0, 0, 128};
  // qr_surface.attr.size       = 37*10;

  if (createQrCodeSurfaceScale(&qr_surface, 10.0) < 0) {
    printf("[ERROR] createQrCodeSurface: %s\n", SDL_GetError());
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return 1;
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    printf("[ERROR] IMG_Init: %s\n", IMG_GetError());
    return 1;
  }

  Uint32 flags       = SDL_WINDOW_RESIZABLE;
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

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, qr_surface.surface);
  if (texture == NULL) {
    printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Point size;
  SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
  printf("Generated QR code: %dx%d, with texture: %dx%d\n", qr_surface.qr.size, qr_surface.qr.size, size.x, size.y);

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
          } else if (event.key.keysym.sym == SDLK_2) {
            printf("%d\n", 2);
          } else if (event.key.keysym.sym == SDLK_3) {
            printf("%d\n", 3);
          }

          break;
        }
        case SDL_MOUSEWHEEL: {
          // if (event.wheel.y > 0) {
          //   scale += 0.1;
          // } else if (event.wheel.y < 0) {
          //   scale -= 0.1;
          // }

          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0xe2, 0x7d, 0x60, 255);
    SDL_RenderClear(renderer);

    SDL_Rect r2 = {0.5 * (INITIAL_WIDTH - qr_surface.attr.size), 0.5 * (INITIAL_HEIGHT - qr_surface.attr.size), qr_surface.attr.size, qr_surface.attr.size};

    SDL_RenderCopy(renderer, texture, NULL, &r2);

    SDL_RenderPresent(renderer);
  }

  destroyQrCodeSurface(&qr_surface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();

  return 0;
}

int createQrCode(QrData* qr) {
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  if (!qrcodegen_encodeText(qr->attr.input, tempBuffer, qr->qrcode, qr->attr.level, qr->attr.version_min, qr->attr.version_max, qr->attr.mask, qr->attr.boost_ecc)) {
    return -1;
  }

  qr->size = qrcodegen_getSize(qr->qrcode);

  return 0;
}

int createQrCodeSurface(QrSurface* qr_surface) {
  if (createQrCode(&qr_surface->qr) < 0) return -1;
  return createQrCodeSurfaceWithSize(qr_surface, qr_surface->attr.size);
}

int createQrCodeSurfaceScale(QrSurface* qr_surface, float scale) {
  if (createQrCode(&qr_surface->qr) < 0) return -1;
  qr_surface->attr.size = (float)qr_surface->qr.size * scale;
  return createQrCodeSurfaceWithSize(qr_surface, qr_surface->attr.size);
}

int createQrCodeSurfaceWithSize(QrSurface* qr_surface, int size) {
  SDL_Surface* tmp_surface = SDL_CreateRGBSurface(0, qr_surface->qr.size, qr_surface->qr.size, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  if (tmp_surface == NULL) {
    return -1;
  }

  SDL_SetSurfaceBlendMode(tmp_surface, SDL_BLENDMODE_BLEND);

  for (int x = 0; x < qr_surface->qr.size; x++) {
    for (int y = 0; y < qr_surface->qr.size; y++) {
      SDL_Rect r1 = {x, y, 1, 1};
      if (qrcodegen_getModule(qr_surface->qr.qrcode, x, y)) {
        SDL_FillRect(tmp_surface, &r1, SDL_MapRGBA(tmp_surface->format, qr_surface->attr.foreground.r, qr_surface->attr.foreground.g, qr_surface->attr.foreground.b, qr_surface->attr.foreground.a));
      } else {
        SDL_FillRect(tmp_surface, &r1, SDL_MapRGBA(tmp_surface->format, qr_surface->attr.background.r, qr_surface->attr.background.g, qr_surface->attr.background.b, qr_surface->attr.background.a));
      }
    }
  }

  qr_surface->surface = SDL_CreateRGBSurface(0, size, size, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  if (qr_surface->surface == NULL) {
    SDL_FreeSurface(tmp_surface);
    return -1;
  }

  SDL_SetSurfaceBlendMode(qr_surface->surface, SDL_BLENDMODE_BLEND);

  if (SDL_BlitScaled(tmp_surface, NULL, qr_surface->surface, NULL) < 0) {
    SDL_FreeSurface(tmp_surface);
    return -1;
  }

  SDL_FreeSurface(tmp_surface);
  return 0;
}

void destroyQrCodeSurface(QrSurface* qr_surface) {
  SDL_FreeSurface(qr_surface->surface);
  qr_surface->surface = NULL;
}