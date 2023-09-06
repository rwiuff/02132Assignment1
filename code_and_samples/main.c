// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>

void grey_scale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int i;
  int j;
  for (i = 0; i <= BMP_WIDTH; i++)
  {
    for (j = 0; j <= BMP_HEIGTH; j++)
    {
      tmp_image[i][j] = (input_image[i][j][0] + input_image[i][j][1] + input_image[i][j][2]) / 3;
    }
  }
}

void binary_threshold(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int i;
  int j;
  int threshold = 90;
  for (i = 0; i <= BMP_WIDTH; i++)
  {
    for (j = 0; j <= BMP_HEIGTH; j++)
    {
      tmp_image[i][j] = tmp_image[i][j] <= threshold ? 0 : 255;
    }
  }
}

void to_rgb(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int i;
  int j;
  for (i = 0; i <= BMP_WIDTH; i++)
  {
    for (j = 0; j <= BMP_HEIGTH; j++)
    {
      output_image[i][j][0] = tmp_image[i][j];
      output_image[i][j][1] = tmp_image[i][j];
      output_image[i][j][2] = tmp_image[i][j];
    }
  }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
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

  printf("Cell Detection Program\n");
  start = clock();
  // Load image from file
  read_bitmap(argv[1], input_image);

  // Convert to grey scale
  grey_scale(input_image, tmp_image);

  // Apply binary threshold
  binary_threshold(tmp_image);

  // Convert to output format for testing purposes
  to_rgb(tmp_image, output_image);

  // Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  end = clock();
  cpu_time_used = (end - start)*1000.0/CLOCKS_PER_SEC;
  printf("Total time: %f ms\n", cpu_time_used);
  return 0;
}
