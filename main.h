#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "SDL.h"
#include "SDL_image.h"
#include "argtable3.h"
#include "qrcodegen.h"

#define MAX_TEXT_INPUT 8192

static const char* APP_NAME          = "qrtool";
static int ALPHA_BACKGROUND_BOX_SIZE = 8;

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

char* safe_strcpy(char* dest, size_t size, const char* src);
void str_tolower(char* input);
bool ishex(char c);
bool str_ishex(const char* input);
int read_stdin(char* buffer, size_t max_size);
bool parse_color(const char* input, SDL_Color* color);
void print_err(void** argtable, size_t len, const char* prog, const char* format, ...);
void free_argtable(void** argtable, size_t len);

#endif