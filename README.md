# qrtool

## Overview

A command line tool that generates QR codes and outputs them as a PNG file. You can supply command line arguments to set the input text, ECC level, mask, and min/max version of the QR code. There are also command line argument that are used to set the outputted image scale and foreground/background color. A helpful command line argument, --verify, is used to visualize what the QR code will look like before saving the image to disk. Also, --quiet, that will supress and text outputted to the terminal, except if any error occurs. You can set the QR code text via stdin or a command line argument. Provide an output file via a command line argument to save the image somewhere on disk.

![qr](./assets/example2.png)
![qr](./assets/example1.png)
![qr](./assets/example3.png)



## Quick Start
```bash
# Building
mkdir build
cd build
cmake .. # It may take a little bit!
make -j 8

# Installing
sudo make install
```

## Examples
    You can look at example.sh if you wish to view some examples.                                          


## command line options:  

##### -t, --text-input=INPUT
The input text that will be used to generate the QR code   
##### -z, --error-correction-level=LEVEL
The Error Correction Level of the QR Code. [LOW | MEDIUM | QUARTILE | HIGH | L | M | Q | H] (Default is high)   
##### -m, --mask=MASK
The mask used to generate the QR code. [0 | 1 | 2 | 3 | 4 | 5 | 6 | 7] (If this option is absent, then the mask is automatically selected)   
##### -a, --boost-ecc
If present, this option will increase the error correction level if needed.   
##### -x, --version-max-range=NUM
The max version of the QR code. (1-40 and if absent, default to 40)   
##### -n, --version-min-range=NUM
The min version of the QR code. (1-40 and if absent, default to 1)   
##### -f, --foreground-color=COLOR
The foreground color of the QR code. Use hex notation: #RRGGBBAA (Default is black)   
##### -b, --background-color=COLOR
The background color of the QR code. Use hex notation: #RRGGBBAA (Default is white   
##### -s, --scale=FLOAT
The scale of the outputted qr code. (Default is 1.0)   
##### -o, --output=FILE
The outputted image of the QR code.   
##### -v, --verify
Show the image before saving the image. Press 'y' to save image and 'n' to cancel saving.   
##### -q, --quiet
Only output text if there is an error. --help and --version will still output text.   
##### --help
print this help and exit   
##### --version
print version information and exit   

## Dependancies
- cmake 3.0.0+
- SDL2
- SDL_image

Cmake will take care of SDL2 and SDL_image, so you really only need the correct cmake version! 

<!-- 
Thoughts:
* Have a batch option for multiple text input. 
-->