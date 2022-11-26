#!/bin/sh


# Input text via command line argument
./build/qrtool                                                 \
--text-input             "https://github.com/REDl3east/qrtool" \
--error-correction-level "high"                                \
--mask                    5                                    \
--version-min-range       10                                   \
--version-max-range       20                                   \
--foreground-color        "#153801ff"                          \
--background-color        "#e5fad9ff"                          \
--scale                   3                                    \
--output                  "./assets/example1.png" 

# Input text via stdin, also add transparency to background, and boost ECC if needed.
# If text is inputted via the keyboard then press Ctrl+D to continue.
printf "%s" "What are you looking at?" | ./build/qrtool        \
--error-correction-level "low"                                 \
--mask                    7                                    \
--version-min-range       1                                    \
--version-max-range       10                                   \
--foreground-color        "#2f3c7e77"                          \
--background-color        "#fbeaeb77"                          \
--scale                   5                                    \
--boost-ecc                                                    \
--output                  "./assets/example2.png" 

# Input text via command line argument and verify the QR code before saving. Aslo supress any outputted text to terminal.
./build/qrtool                                                 \
--text-input             "This is some really long text. There once was a guy named Jim who lived in the middle of Nowhere. He lived with a nice pink dog named Aaudacious. :)" \
--error-correction-level "high"                                \
--mask                    3                                    \
--version-min-range       10                                   \
--version-max-range       20                                   \
--foreground-color        "#ee4e34ff"                          \
--background-color        "#fceddaff"                          \
--scale                   3                                    \
--output                  "./assets/example3.png"              \
--verify                                                       \
--quiet