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
// Start by implying Erosion will cause change to the picture
unsigned char change=1;

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

  unsigned char t_opt=90;
  binary_threshold(tmp_image, t_opt);

  // Initialise counters for terminal writeout.
  int count = 0;  // Counted cells.
  unsigned char step = 0;   // Iteration steps.
  printf("|-----------------------------|\n");
  printf("|   Cell Detection Program    |\n");
  printf("|-----------------------------|\n");
  // Do-While loop for erosion
  do // Do the following:
  {
	step++;
  // Perform erosion. If returns a 0 because of no changes occured, change var change to 0 to stop the loop
    if(erode(tmp_image)){}else { change=0; }
                                     
    
          
                                              // Increment step count.
  } while (change);                                     // Until no change has occured 
  count = detection(tmp_image, image);
  
  printf("|-----------------------------|\n");
  printf("| Steps: %3d                  |\n", step);
  printf("| Total detected cells: %3d   |\n", count);
  printf("|-----------------------------|\n");
  // Save image to file
  write_bitmap(image, argv[2]);

  printf("| Done!                       |\n");
  end = clock();
  cpu_time_used = (end - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("| Total time: %.0f ms         |\n", cpu_time_used);
  printf("|-----------------------------|\n");
  return 0;
}
