
# PPMToBMP

A simple C PPM to BMP converter parallelized with OpenMP.


## Build

This application can be built by simply running the ```make``` command from within this directory.


## Usage

The PPM to BMP application can be run using the following command.

```./ppmToBMP <input> <output> <cores> <display>```

The parameters that follow apply.

+ Input   - a .ppm image upon which to perform the operation.
+ Output  - a .ppm image to output.
+ Cores   - the number of cores to utilize for this parallel operation.
+ Display - 0 for timing, 1 for full report, and 2 for color search percentages.

Example:  ```./ppmToBMP input.ppm output.bmp 4 1```


## Conversion

One may convert to and from .ppm images using the Unix terminal command:

```convert <filenameIn>.<extensionIn> <filenameOut>.<extensionOut>```

This app is designed for systems without such libraries.
