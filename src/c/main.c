
/*======================================
    main.c
    GKSuite Application Runner
  ======================================
    This template can be extended to
      many other apps.  Just change:
        1) Inclusions
        2) Number of Arguments
        3) App Name
        4) Usage and Argument Parsing
        5) Function Call
    This variant supports image input
      and output.  Other variants for
      output only, input only, dual-
      input single-output, etc. are
      available.
  ======================================*/


// Inclusions
#include <inttypes.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ppmToBMP.h"
#include "ppmTools.h"
#include "qdbmp.h"


// Definitions
#define NUM_ARGS 5

// Main
int main( int argc, char **argv ) {

  // General
  const char    *name          = "PPM To BMP";
        uint8_t  i             = 0;                  // Loop Iterator
        uint8_t  cores         = 0;                  // Number of Cores to Utilize
        uint8_t  display       = 0;                  // Argument to Display Debug Text
  // Image
        uint8_t *image         = NULL;               // Image Pointer - RGB888
        BMP     *bmpImage      = NULL;               // BMP Pointer for QDBMP Image Library
  const char    *in            = NULL;               // Input Filename
  const char    *out           = NULL;               // Output Filename
        uint16_t width         = 0;                  // Image Width
        uint16_t height        = 0;                  // Image Height
  // Timing Components
        double   jobStart      = 0.0;                // Job Clock Start Time
        double   imageInStart  = 0.0;                // Image Input Start Time
        double   imageInEnd    = 0.0;                // Image Input End Time
        double   opStart       = 0.0;                // Operation Start Time
        double   opEnd         = 0.0;                // Operation End Time
        double   imageOutStart = 0.0;                // Image Output Start Time
        double   imageOutEnd   = 0.0;                // Image Output End Time
        double   jobEnd        = 0.0;                // Job Clock End Time
  // Final Time Values
        double   imageInTime   = 0.0;                // Image Input and Preparation Time
        double   opTime        = 0.0;                // Operation Execution Time
        double   imageOutTime  = 0.0;                // Image Output Time
        double   addOverhead   = 0.0;                // Additional Overhead Time
        double   jobTime       = 0.0;                // Total Program Time

  // Input Parsing and Error Handling
  if( argc == NUM_ARGS ) {
    // Parsing Normal Arguments
    in      = argv[1];
    out     = argv[2];
    cores   = atoi( argv[3] );
    display = atoi( argv[4] );
  // If Argument Number Does Not Fall Into Acceptable Range, Print Usage
  } else {
    fprintf( stderr, "Usage: %s <input> <output> <cores> <display>\n", argv[0] );
    fprintf( stderr, "  Input   - input .ppm file.\n" );
    fprintf( stderr, "  Output  - desired output filename.ppm.\n" );
    fprintf( stderr, "  Cores   - number of cores to utilize for parallel operation\n" );
    fprintf( stderr, "            or zero for a serial baseline run.\n" );
    fprintf( stderr, "  Display - 0 for timing, 1 for full report.\n" );
    fprintf( stderr, "Example:  %s input.ppm output.ppm 4 1\n\n", argv[0] );
    return 0;
  }

  // Intro Message
  if( display == 1 ) {
    fprintf( stdout, "\n   = = =  %s  = = =   \n\n", name );
  }

  // OpenMP Test
  if( display == 1 ) {
    if( cores != 0 ) {
      fprintf( stdout, "Using %d Cores of Maximum %d Cores Available\nTesting - Report\n", cores, omp_get_max_threads( ) );
      #pragma omp parallel for num_threads( cores )
      for( i = 0; i < omp_get_num_threads( ); i++ ) {
        fprintf( stdout, "  Core %d of %d Reporting!\n", omp_get_thread_num( ), omp_get_num_threads( ) );
      }
    } else {
      fprintf( stdout, "Running Serial Baseline\n" );
    }
  }

  // Job Start Timer
  jobStart     = omp_get_wtime( );

  // Image Allocation
  if( display == 1 ) {
    fprintf( stdout, "Initializing Image...\n" );
  }
  imageInStart = omp_get_wtime( );
  image = readPPM( in, &width, &height );
  if( !image ) {
    fprintf( stderr, "Failed to Initialize Image\n" );
    return 1;
  }
  bmpImage = BMP_Create( width, height, 24 );
  BMP_CHECK_ERROR( stderr, -3 );
  imageInEnd   = omp_get_wtime( );

  // Application Operation
  if( display == 1 ) {
    fprintf( stdout, "Performing %s Operation...\n", name );
  }
  if( cores == 0 ) {
    opStart      = omp_get_wtime( );
    ppmToBMPSerial( image, bmpImage, width, height );
    opEnd      = omp_get_wtime( );
  } else {
    opStart = omp_get_wtime( );
    ppmToBMPOMP( image, bmpImage, width, height, cores );
    opEnd   = omp_get_wtime( );
  }


  // Image Write
  if( display == 1 ) {
    fprintf( stdout, "Writing Image...\n" );
  }
  imageOutStart = omp_get_wtime( );
  BMP_WriteFile( bmpImage, out );
  BMP_CHECK_ERROR( stderr, -5 );
  imageOutEnd   = omp_get_wtime( );

  // Final Tasks
  free( image );
  BMP_Free( bmpImage );
  if( display == 1 ) {
    fprintf( stdout, "Operation Complete!\n\n" );
  }

  // Final Timing Component
  jobEnd       = omp_get_wtime( );                               // End Full Program Clock
  imageInTime  = imageInEnd  - imageInStart;                     // Calculate Image Input and Preparation Time
  opTime       = opEnd       - opStart;                          // Calculate Operation Time
  imageOutTime = imageOutEnd - imageOutStart;                    // Calculate Image Output Time
  jobTime      = jobEnd      - jobStart;                         // Calculate Overall Time
  addOverhead  = jobTime - imageInTime - opTime - imageOutTime;  // Calculate Additional Overhead Time
  // Handle Displaying All Timing Data
  if( display == 1 ) {  // Messages only when display is 1.
    fprintf( stdout, "=== Timing Data ===\n  Image In:\t\t" );
    fprintf( stdout, "%0.7lf\n", imageInTime );                                                                                                                  fprintf( stdout, "  Operation:\t\t" );
  }
  if( display <= 1 ) {  // Gives Timing Regardless
    fprintf( stdout, "%0.7lf ", opTime );
  }
  if( display == 1 ) {
    fprintf( stdout, "\n  Image Out:\t\t" );
    fprintf( stdout, "%0.7lf\n", imageOutTime );
    fprintf( stdout, "  Overhead:\t\t" );
    fprintf( stdout, "%0.7lf\n", addOverhead );
    fprintf( stdout, "  Total Job Time:\t" );
    fprintf( stdout, "%0.7lf\n\n", jobTime );
  }
  // Define Additional Output Functions with Different Display Values

  return 0;

}



// END main.c  - EWG SDG
