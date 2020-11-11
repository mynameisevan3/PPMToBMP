
/*=========================================
    ppmToBMP.c
    PPM To BMP Function Implementations
  =========================================*/


// Inclusions
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppmToBMP.h"
#include "qdbmp.h"


// Function Implementations

// ppmToBMPSerial - given an input image, output BMP image, width and height, performs the data
//   movement operations for conversion.
uint8_t ppmToBMPSerial( uint8_t *image, BMP *bmpImage, uint16_t width, uint16_t height ) {

  uint16_t x = 0;  // BMP Image X Axis
  uint16_t y = 0;  // BMP Image Y Axis
  uint32_t i = 0;  // Input Image Pixel Loop Iterator

  // Setting BMP Pixels
  for( y = 0; y < height; y++ ) {
    for( x = 0; x < width; x++ ) {
      i = ( y * width + x ) * 3;
      BMP_SetPixelRGB( bmpImage, x, y, (char)( image[i] ), (char)( image[i+1] ), (char)( image[i+2] ) );
    }
  }

  return 0;

}


// ppmToBMPOMP - same as above, parallelized with OpenMP.
uint8_t ppmToBMPOMP( uint8_t *image, BMP *bmpImage, uint16_t width, uint16_t height, uint8_t cores ) {
  uint16_t x = 0;  // BMP Image X Axis
  uint16_t y = 0;  // BMP Image Y Axis
  uint32_t i = 0;  // Input Image Pixel Loop Iterator

  #pragma omp parallel for default( none ) num_threads( cores ) schedule( static ) \
            shared( image, bmpImage, width, height ) \
            private( x, y, i )

  // Setting BMP Pixels
  for( y = 0; y < height; y++ ) {
    for( x = 0; x < width; x++ ) {
      i = ( y * width + x ) * 3;
      BMP_SetPixelRGB( bmpImage, x, y, (char)( image[i] ), (char)( image[i+1] ), (char)( image[i+2] ) );
    }
  }

  return 0;

}



// END ppmToBMP.c  - EWG SDG
