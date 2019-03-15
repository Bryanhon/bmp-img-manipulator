# bmp-img-manipulator
A .bmp file image manipulator

# Usage

## Building

To use this program first compile it with the given Makefile by simply running
"make". If the program compiles successfully it should output an executable
called "bmp".

## Running

On UNIX style operating systems simply run the following command:

```
./bmp -i images/input.bmp -o output.bmp
```

This will invert the image and create a new .bmp file called "output.bmp".

## Options

```
-h or --help: Displays this information.
-v or --verbose: Verbose mode.
-i or --invert: invert.
-r or --rotate integer: Rotate in orders of 90. (NOT IMPLEMENTED)
-o or --outputfile string: outputfile.
-f or --filter integer: apply filter to the image.
filter codes are:
1 = sepia
2 = greyscale
NOTE: No 2 filters can be applied at the same time
```

