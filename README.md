# qrtool

## Quick Start
```bash
mkdir build
cd build
cmake .. # It may take a little bit!
make -j 8
```

## Dependancies
- cmake 3.0.0+
- SDL2
- SDL_image

Cmake will take care of SDL2 and SDL_image, so you really only need the correct cmake version! 

## Idea

A command line tool that generates cool qr codes.

### command line options:  

-t, --input-text INPUT (Input should also be obtained via stdin. If this option is absent, then it will grab data from stdin.)  
-z, --error-correction-level [LOW | MEDIUM | QUARTILE | HIGH | L | M | Q | H]  
-m, --mask [0 | 1 | 2 | 3 | 4 | 5 | 6 | 7] (If this option is absent, then the mask is automatically selected.)  
-a, --boost-ecc (Will increase the error correction level if needed.)  
-n, --version-max-range (1-40) (If absent, set to 1.)  
-x, --version-min-range (1-40) (If absent, set to 40.)  

-f, --foreground [HEX | RGB() | RGBA()]  
-b, --background [HEX | RGB() | RGBA()]  

-s, --scale (The scale of the outputted qr code. Default is 1)  

-o, --output (Input can be outputted to an image file (png, jpeg, etc) or if this option is absent, then it will be outputted to stdout. If -e is absent, then the extension is inferred from the given string. If no extension is found, then it is set to a default, probably PPM6.)
-e, --extension [PNG | JPEG | etc] (Extension of the file. Use this without -o option and the binary data will be outputted to stdout.)    
-v, --verify  (View the image in a window without saving it in any way. Pressing 'y' will save then quit, and 'n' will quit.)   

### Thoughts

* Make scale option have decimal abilities.
* Have a batch option for multiple text input.