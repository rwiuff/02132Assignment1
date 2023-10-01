// To compile (linux/mac): gcc functions.c cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_out.bmp

// To compile (win): gcc functions.c cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_out.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>
#include "functions.h"

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
// unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH];

// Main function
int main(int argc, char **argv)
{
  clock_t start, end;
  double cpu_time_used;
  // argc counts how may arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  // Start CPU clock.
  start = clock();

  // Load image from file
  read_bitmap(argv[1], image);

  // Convert to grey scale
  grey_scale(image, tmp_image);

  // Apply binary threshold
  int t_opt = 90;
  binary_threshold(tmp_image, t_opt);

  // Initialise counters for terminal writeout.
  int pixels = 0; // White pixels left in picture.
  int count = 0;  // Counted cells.
  int step = 0;   // Iteration steps.
  int captureSize = 12;
  printf("#-----------------------------#\n");
  printf("|   Cell Detection Program    |\n");
  printf("|-----------------------------|\n");
  // Do-While loop for erosion and detection.
  do // Do the following:
  {
    pixels = pixelCheck(tmp_image);                          // Count white pixels.
    erode(tmp_image);                                        // Perform erosion.
    count = detection(image, tmp_image, count, captureSize); // Detect cells and add to counter.
    step++;                                                  // Increment step count.
  } while (pixels != 0);                                     // Until no whities are left.

  // Convert to output format for testing purposes
  printf("|-----------------------------|\n");
  printf("| Erosion and detection       |\n");
  printf("| iterations: %3d             |\n", step);
  printf("|                             |\n");
  printf("| Total detected cells: %3d   |\n", count);
  printf("|-----------------------------|\n");
  // Save image to file
  write_bitmap(image, argv[2]);

  printf("| Done!                       |\n");
  end = clock();
  cpu_time_used = (end - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("| Total time: %.0f ms         |\n", cpu_time_used);
  printf("#-----------------------------#\n");
  return 0;
}
