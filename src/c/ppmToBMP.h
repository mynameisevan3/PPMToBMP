
/*=================================
    ppmToBMP.h
    PPM To BMP Function Headers
  =================================*/


// Include Guard
#ifndef PPMTOBMP_H
#define PPMTOBMP_H


// Inclusions
#include <stdint.h>
#include "qdbmp.h"


// Function Prototypes
uint8_t ppmToBMPSerial( uint8_t *image, BMP *bmpImage, uint16_t width, uint16_t height );

uint8_t ppmToBMPOMP( uint8_t *image, BMP *bmpImage, uint16_t width, uint16_t height, uint8_t cores );



// End Include Guard
#endif



// END ppmToBMP.h  - EWG SDG
