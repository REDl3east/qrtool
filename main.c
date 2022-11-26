#include "main.h"

int main(int argc, char** argv) {
  char text_input[MAX_TEXT_INPUT];

  struct arg_str* text_input_arg  = arg_str0("t", "text-input", "INPUT", "The input text that will be used to generate the QR code");
  struct arg_str* qr_level_arg    = arg_str0("z", "error-correction-level", "LEVEL", "The Error Correction Level of the QR Code. [LOW | MEDIUM | QUARTILE | HIGH | L | M | Q | H] (Default is high)");
  struct arg_int* qr_mask_arg     = arg_int0("m", "mask", "MASK", "The mask used to generate the QR code. [0 | 1 | 2 | 3 | 4 | 5 | 6 | 7] (If this option is absent, then the mask is automatically selected)");
  struct arg_lit* qr_boost_arg    = arg_lit0("a", "boost-ecc", "If present, this option will increase the error correction level if needed.");
  struct arg_int* qr_max_arg      = arg_int0("x", "version-max-range", "NUM", "The max version of the QR code. (1-40 and if absent, default to 40)");
  struct arg_int* qr_min_arg      = arg_int0("n", "version-min-range", "NUM", "The min version of the QR code. (1-40 and if absent, default to 1)");
  struct arg_str* qr_fg_color_arg = arg_str0("f", "foreground-color", "COLOR", "The foreground color of the QR code. Use hex notation: #RRGGBBAA (Default is black)");
  struct arg_str* qr_bg_color_arg = arg_str0("b", "background-color", "COLOR", "The background color of the QR code. Use hex notation: #RRGGBBAA (Default is white");
  struct arg_str* qr_scale_arg    = arg_str0("s", "scale", "FLOAT", "The scale of the outputted qr code. (Default is 1.0)");
  struct arg_file* qr_out_arg     = arg_file1("o", "output", "FILE", "The outputted image of the QR code.");
  struct arg_lit* qr_verify_arg   = arg_lit0("v", "verify", "Show the image before saving the image. Press 'y' to save image and 'n' to cancel saving.");
  struct arg_lit* qr_quiet_arg    = arg_lit0("q", "quiet", "Only output text if there is an error. --help and --version will still output text.");

  struct arg_lit* help    = arg_lit0(NULL, "help", "print this help and exit");
  struct arg_lit* version = arg_lit0(NULL, "version", "print version information and exit");
  struct arg_end* end     = arg_end(20);

  void* argtable[]    = {text_input_arg, qr_level_arg, qr_mask_arg, qr_boost_arg, qr_max_arg, qr_min_arg, qr_fg_color_arg, qr_bg_color_arg, qr_scale_arg, qr_out_arg, qr_verify_arg, qr_quiet_arg, help, version, end};
  size_t argtable_len = sizeof(argtable) / sizeof(argtable[0]);

  if (arg_nullcheck(argtable) != 0) {
    fprintf(stderr, "[ERROR] %s: insufficient memory\n", argv[0]);
    return 1;
  }

  int err = arg_parse(argc, argv, argtable);

  if (help->count > 0) {
    printf("Usage: %s", argv[0]);
    arg_print_syntax(stdout, argtable, "\n");
    printf("A command line tool that generates QR codes and outputs them as a PNG file.\n");

    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    return 0;
  }

  if (version->count > 0) {
    printf("'%s' A command line tool that generates QR codes and outputs them as a PNG file.\n", argv[0]);
    printf("2022, Dalton Overmyer\n");
    return 0;
  }

  if (err > 0) {
    arg_print_errors(stdout, end, argv[0]);
    return 1;
  }

  QrSurface qr_surface;
  qr_surface.qr.attr.input       = text_input;
  qr_surface.qr.attr.level       = qrcodegen_Ecc_HIGH;
  qr_surface.qr.attr.mask        = qrcodegen_Mask_AUTO;
  qr_surface.qr.attr.version_min = qrcodegen_VERSION_MIN;
  qr_surface.qr.attr.version_max = qrcodegen_VERSION_MAX;
  qr_surface.qr.attr.boost_ecc   = 0;

  qr_surface.attr.background = (SDL_Color){255, 255, 255, 255};
  qr_surface.attr.foreground = (SDL_Color){0, 0, 0, 255};

  if (text_input_arg->count > 0) {
    safe_strcpy(text_input, MAX_TEXT_INPUT, text_input_arg->sval[0]);
    if (text_input == NULL) {
      fprintf(stderr, "[ERROR] safe_strcpy failed\n");
      return 1;
    }
  } else {
    if (read_stdin(text_input, MAX_TEXT_INPUT) < 0) {
      fprintf(stderr, "[ERROR] read failed: %s\n", strerror(errno));
      return 1;
    }
    printf("\n");
  }

  if (qr_level_arg->count > 0) {
    char* level_str = strdup(qr_level_arg->sval[0]);

    str_tolower(level_str);

    if (strcmp(level_str, "l") == 0 || strcmp(level_str, "low") == 0) {
      qr_surface.qr.attr.level = qrcodegen_Ecc_LOW;
    } else if (strcmp(level_str, "m") == 0 || strcmp(level_str, "medium") == 0) {
      qr_surface.qr.attr.level = qrcodegen_Ecc_MEDIUM;
    } else if (strcmp(level_str, "q") == 0 || strcmp(level_str, "quartile") == 0) {
      qr_surface.qr.attr.level = qrcodegen_Ecc_QUARTILE;
    } else if (strcmp(level_str, "h") == 0 || strcmp(level_str, "high") == 0) {
      qr_surface.qr.attr.level = qrcodegen_Ecc_HIGH;
    } else {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Error Correction Level: %s\n", level_str);
      free(level_str);
      return 1;
    }

    free(level_str);
  }

  if (qr_mask_arg->count > 0) {
    int qr_mask = *qr_mask_arg->ival;
    if (qr_mask < 0 || qr_mask > qrcodegen_Mask_7) {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Mask: %d\n", qr_mask);
      return 1;
    }
    qr_surface.qr.attr.mask = qr_mask;
  }

  if (qr_boost_arg->count > 0) {
    qr_surface.qr.attr.boost_ecc = 1;
  }

  if (qr_min_arg->count > 0) {
    int qr_min = *qr_min_arg->ival;
    if (qr_min < 1 || qr_min > 40) {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Min Version: %d\n", qr_min);
      return 1;
    }
    qr_surface.qr.attr.version_min = qr_min;
  }

  if (qr_max_arg->count > 0) {
    int qr_max = *qr_max_arg->ival;
    if (qr_max < 1 || qr_max > 40) {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Min Version: %d\n", qr_max);
      return 1;
    }
    qr_surface.qr.attr.version_max = qr_max;
  }

  if (qr_surface.qr.attr.version_min > qr_surface.qr.attr.version_max) {
    print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Version: %d(min) > %d(max)\n", qr_surface.qr.attr.version_min, qr_surface.qr.attr.version_max);
    return 1;
  }

  if (qr_fg_color_arg->count > 0) {
    const char* fg_str = qr_fg_color_arg->sval[0];

    if (!parse_color(fg_str, &qr_surface.attr.foreground)) {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Color: %s\n", fg_str);
      return 1;
    }
  }

  if (qr_bg_color_arg->count > 0) {
    const char* bg_str = qr_bg_color_arg->sval[0];

    if (!parse_color(bg_str, &qr_surface.attr.background)) {
      print_err(argtable, argtable_len, argv[0], "[ERROR] Invalid Color: %s\n", bg_str);
      return 1;
    }
  }

  if (qr_quiet_arg->count <= 0) {
    printf("Generating QR code with attributes:\n");
    printf("   Input:       %s\n", qr_surface.qr.attr.input);
    printf("   ECC Level:   %d\n", qr_surface.qr.attr.level);
    if (qr_surface.qr.attr.mask != -1) {
      printf("   Mask:        %d\n", qr_surface.qr.attr.mask);
    } else {
      printf("   Mask:        auto\n");
    }
    printf("   Version Min: %d\n", qr_surface.qr.attr.version_min);
    printf("   Version Max: %d\n", qr_surface.qr.attr.version_max);
    printf("   Boost ECC:   %s\n", qr_surface.qr.attr.boost_ecc ? "true" : "false");
  }

  float qr_scale = 1.0;
  if (qr_scale_arg->count > 0) {
    const char* qr_scale_str = qr_scale_arg->sval[0];
    qr_scale                 = atof(qr_scale_str);
    if (qr_scale < 1.0) qr_scale = 1.0;
    if (createQrCodeSurfaceScale(&qr_surface, qr_scale) < 0) {
      print_err(argtable, argtable_len, "[ERROR] createQrCodeSurface: %s\n", SDL_GetError());
      return 1;
    }
  } else {
    if (createQrCodeSurfaceScale(&qr_surface, 1.0) < 0) {
      print_err(argtable, argtable_len, "[ERROR] createQrCodeSurface: %s\n", SDL_GetError());
      return 1;
    }
  }

  if (qr_quiet_arg->count <= 0) printf("Generated QR code [%dx%d]\n\n", qr_surface.qr.size, qr_surface.qr.size);

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    fprintf(stderr, "[ERROR] IMG_Init: %s\n", IMG_GetError());
    free_argtable(argtable, argtable_len);
    destroyQrCodeSurface(&qr_surface);
    return 1;
  }

  bool qr_save = true;

  if (qr_verify_arg->count > 0) {
    if (qr_quiet_arg->count <= 0) {
      printf("Verifying...\n");
      printf("Press 'y' to save image (%s) then quit, or press 'n' to quit\n\n", qr_out_arg->filename[0]);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "[ERROR] %s\n", SDL_GetError());
      free_argtable(argtable, argtable_len);
      destroyQrCodeSurface(&qr_surface);
      IMG_Quit();
      return 1;
    }

    Uint32 flags       = SDL_WINDOW_RESIZABLE;
    SDL_Window* window = SDL_CreateWindow(APP_NAME, 0, 0, qr_surface.attr.size, qr_surface.attr.size, flags);

    if (!window) {
      fprintf(stderr, "[ERROR] SDL_CreateWindow: %s\n", SDL_GetError());
      free_argtable(argtable, argtable_len);
      destroyQrCodeSurface(&qr_surface);
      IMG_Quit();
      SDL_Quit();
      return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
      fprintf(stderr, "[ERROR] SDL_CreateRenderer: %s\n", SDL_GetError());
      free_argtable(argtable, argtable_len);
      destroyQrCodeSurface(&qr_surface);
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, qr_surface.surface);
    if (texture == NULL) {
      fprintf(stderr, "[ERROR] SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
      free_argtable(argtable, argtable_len);
      destroyQrCodeSurface(&qr_surface);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return 1;
    }

    int done = 0;

    while (!done) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if ((event.type == SDL_QUIT) ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
            (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))) {
          qr_save = false;
          done    = 1;
        }
        switch (event.type) {
          case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_y) {
              qr_save = true;
              done    = true;
            } else if (event.key.keysym.sym == SDLK_n) {
              qr_save = false;
              done    = true;
            }

            break;
          }
        }
      }

      SDL_SetRenderDrawColor(renderer, 0xe2, 0x7d, 0x60, 255);
      SDL_RenderClear(renderer);

      for (int x = 0; x < (qr_surface.attr.size / ALPHA_BACKGROUND_BOX_SIZE) + 1; x++) {
        for (int y = 0; y < (qr_surface.attr.size / ALPHA_BACKGROUND_BOX_SIZE) + 1; y++) {
          SDL_Rect r1 = {x * ALPHA_BACKGROUND_BOX_SIZE, y * ALPHA_BACKGROUND_BOX_SIZE, ALPHA_BACKGROUND_BOX_SIZE, ALPHA_BACKGROUND_BOX_SIZE};

          if (x % 2 == 0) {
            if (y % 2 == 0) {
              SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255);
            } else {
              SDL_SetRenderDrawColor(renderer, 153, 153, 153, 255);
            }
          } else {
            if (y % 2 == 0) {
              SDL_SetRenderDrawColor(renderer, 153, 153, 153, 255);
            } else {
              SDL_SetRenderDrawColor(renderer, 102, 102, 102, 255);
            }
          }

          SDL_RenderFillRect(renderer, &r1);
        }
      }

      SDL_Rect r2 = {0, 0, qr_surface.attr.size, qr_surface.attr.size};
      SDL_RenderCopy(renderer, texture, NULL, &r2);
      SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  if (qr_save) {
    if (qr_quiet_arg->count <= 0) {
      printf("Generating %s with attributes:\n", qr_out_arg->filename[0]);
      printf("   Scale:      %f\n", qr_scale);
      printf("   Size:       %d\n", qr_surface.surface->w);
      printf("   Foreground: RGBA(%d, %d, %d, %d)\n", qr_surface.attr.foreground.r, qr_surface.attr.foreground.g, qr_surface.attr.foreground.b, qr_surface.attr.foreground.a);
      printf("   Background: RGBA(%d, %d, %d, %d)\n", qr_surface.attr.background.r, qr_surface.attr.background.g, qr_surface.attr.background.b, qr_surface.attr.background.a);
    }

    if (IMG_SavePNG(qr_surface.surface, qr_out_arg->filename[0]) < 0) {
      fprintf(stderr, "Failed to save %s\n", qr_out_arg->filename[0]);
      free_argtable(argtable, argtable_len);
      destroyQrCodeSurface(&qr_surface);
      IMG_Quit();
      return 1;
    } else {
      if (qr_quiet_arg->count <= 0)
        printf("Generated %s\n", qr_out_arg->filename[0]);
    }
  } else {
    if (qr_quiet_arg->count <= 0)
      printf("Cancelled\n");
  }

  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  destroyQrCodeSurface(&qr_surface);
  IMG_Quit();

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

// https://stackoverflow.com/questions/41869803/what-is-the-best-alternative-to-strncpy by chqrlie
char* safe_strcpy(char* dest, size_t size, const char* src) {
  if (size > 0) {
    size_t i;
    for (i = 0; i < size - 1 && src[i]; i++) {
      dest[i] = src[i];
    }
    dest[i] = '\0';
  }
  return dest;
}

void str_tolower(char* input) {
  for (int i = 0; input[i]; i++) {
    input[i] = tolower(input[i]);
  }
}

bool ishex(char c) {
  return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
         c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
}

bool str_ishex(const char* input) {
  for (int i = 0; input[i]; i++) {
    if (!ishex(tolower(input[i]))) return false;
  }
  return true;
}

int read_stdin(char* buffer, size_t max_size) {
  int total_bytes_read = 0;
  int nbytes_read      = 0;

  while (1) {
    nbytes_read = read(0, buffer + total_bytes_read, max_size - total_bytes_read);
    total_bytes_read += nbytes_read;

    if (nbytes_read == 0) {
      buffer[total_bytes_read] = '\0';
      break;
    } else if (total_bytes_read >= max_size) {
      buffer[max_size] = '\0';

      // flush stdin so it doesn't pollute the terminal
      int c;
      while ((c = getchar()) != '\n' && c != EOF) {
      }
      break;
    }

    if (nbytes_read < 0) return -1;
  }
  return total_bytes_read;
}

bool parse_color(const char* input, SDL_Color* color) {
  int input_len = strlen(input);

  if (input_len != 9 || input[0] != '#') {
    return false;
  }

  if (!str_ishex(input + 1)) {
    return false;
  }

  unsigned long value = strtoul(input + 1, NULL, 16);

  color->a = (value >> 0) & 0xff;
  color->b = (value >> 8) & 0xff;
  color->g = (value >> 16) & 0xff;
  color->r = (value >> 24) & 0xff;

  return true;
}

void print_err(void** argtable, size_t len, const char* prog, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "Usage: %s", prog);
  arg_print_syntax(stdout, argtable, "\n");
  arg_print_glossary(stdout, argtable, "  %-25s %s\n");
  printf("%lu\n", len);
  arg_freetable(argtable, len);
}

void free_argtable(void** argtable, size_t len) {
  arg_freetable(argtable, len);
}