// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <time.h>
#include <math.h>

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
// unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH];

// Greyscale conversion
void grey_scale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int i = 0; i <= BMP_WIDTH; i++) // Iterate over width
  {
    for (int j = 0; j <= BMP_HEIGTH; j++) // Iterate over height
    {
      tmp_image[i][j] = (input_image[i][j][0] + input_image[i][j][1] + input_image[i][j][2]) / 3; // From the 3 input channels of a given pixel
                                                                                                  // Take the average of the colours and put into array.
    }
  }
}

int inBounds(int x, int y)
{
  return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

void drawX(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
  int thickness = 1;
  for (int dx = -5; dx <= 5; dx++)
  {
    for (int dy = -5; dy <= 5; dy++)
    {
      if (inBounds(x + dx, y + dy))
      {
        if ((dx >= -thickness && dx <= thickness) || (dy >= -thickness && dy <= thickness))
        {
          image[x + dx][y + dy][0] = 255;
          image[x + dx][y + dy][1] = 0;
          image[x + dx][y + dy][2] = 0;
        }
      }
    }
  }
}

// Detection of pixels in detection frame
int frameDetection(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  int clear = 0; // Variable indicating the frame is clear.
  // Set of coordinates for the frame around a picture.
  int frameMask[52][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {1, 0}, {1, 13}, {2, 0}, {2, 13}, {3, 0}, {3, 13}, {4, 0}, {4, 13}, {5, 0}, {5, 13}, {6, 0}, {6, 13}, {7, 0}, {7, 13}, {8, 0}, {8, 13}, {9, 0}, {9, 13}, {10, 0}, {10, 13}, {11, 0}, {11, 13}, {12, 0}, {12, 13}, {13, 0}, {13, 1}, {13, 2}, {13, 3}, {13, 4}, {13, 5}, {13, 6}, {13, 7}, {13, 8}, {13, 9}, {13, 10}, {13, 11}, {13, 12}, {0, 13}};
  for (int x = 0; x < 52; x++) // Iterate over frame pixels
  {
    if (tmp_image[i + frameMask[x][0]][j + frameMask[x][1]] == 255) // If the frame pixels contain a white pixel.
    {
      clear = 1; // Indicate frame is NOT clear.
      break;     // Break the search.
    }
  }
  return clear;
}

// Detection of pixels in capture area.
int cellCapture(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  int detected = 0; // Variable indicating contents in capture area.

  for (int k = i + 1; k <= i + 12; k++) // Iterate within cell area columns.
  {
    for (int l = j + 1; l <= j + 12; l++) // Iterate within cell area rows.
    {
      if (tmp_image[k][l] == 255) // If a white pixel exists.
      {
        detected = 1; // Set contents flag.
      }
    }
  }
  return detected;
}

// Erase discovered cells
void erase(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  for (int k = i; k <= i + 13; k++) // Iterate over cell area columns
  {
    for (int l = j; l <= j + 13; l++) // Iterate over cell area rows
    {
      if (inBounds(k, l) == 1)
      {
        tmp_image[k][l] = 0; // Zerorise pixel value.
        // printf("Deleting pixel (%d,%d)\n", k,l);
      }
    }
  }
}

// Detection method
int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int count = 0;                      // Cell counter.
  for (int i = 0; i < BMP_WIDTH; i++) // Iterate over picture columns.
  {
    for (int j = 0; j < BMP_HEIGTH; j++) // Iterate over picture rows.
    {
      if (frameDetection(tmp_image, i, j) != 1) // Check if a given frame is clear.
      {
        if (cellCapture(tmp_image, i, j) == 1) // Check if a clear frame contains a cell.
        {
          count++; // Increment cell counter.
          // printf("Drawin cell: %d\n", count);
          drawX(image, i + 7, j + 7); // Draw a cross on the detected cell.
          // printf("Erasing cell: %d\n", count);
          erase(tmp_image, i, j); // Clear the area of white pixels.
        }
      }
    }
  }
  return count; // Return this iterations counted cells.
}

// Binary threshold method.
void binary_threshold(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int t_opt)
{
  // int threshold = 90;                  // Initial threshold.
  for (int i = 0; i <= BMP_WIDTH; i++) // Iterate over columns.
  {
    for (int j = 0; j <= BMP_HEIGTH; j++) // Iterate over rows.
    {
      tmp_image[i][j] = tmp_image[i][j] <= t_opt ? 0 : 255; // Ternary operator: If a pixel intensity is below or equal to
    }                                                       // threshold, set it to zero, otherwise 255
  }
}

// Method to split a greyscale into three channels.
void to_rgb(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int i = 0; i <= BMP_WIDTH; i++)
  {
    for (int j = 0; j <= BMP_HEIGTH; j++)
    {
      output_image[i][j][0] = tmp_image[i][j];
      output_image[i][j][1] = tmp_image[i][j];
      output_image[i][j][2] = tmp_image[i][j];
    }
  }
}

// Erosion method
void erode(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char ErosionMap[BMP_WIDTH][BMP_HEIGTH] = {0};
  for (int i = 1; i <= (BMP_WIDTH - 1); i++)
  {
    tmp_image[i - 1][0] = 0;
    tmp_image[i - 1][BMP_HEIGTH - 1] = 0;

    tmp_image[0][i - 1] = 0;
    tmp_image[BMP_WIDTH][i - 1] = 0;
    for (int j = 1; j <= (BMP_HEIGTH - 1); j++)
    {

      if (((tmp_image[i - 1][j] == 0 || tmp_image[i + 1][j] == 0) || (tmp_image[i][j + 1] == 0 || tmp_image[i][j - 1] == 0)))
      {
        ErosionMap[i][j] = 0;
      }
      else
      {
        ErosionMap[i][j] = 255;
      }
    }
  }

  for (int o = 0; o < BMP_WIDTH; o++)
  {
    for (int p = 0; p < BMP_HEIGTH; p++)
    {
      if (ErosionMap[o][p] == 0)
      {
        tmp_image[o][p] = 0;
      }
    }
  }
}

// Count white pixels
int pixelCheck(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int pixels = 0;                     // Zerorised counter.
  for (int i = 0; i < BMP_WIDTH; i++) // For each row.
  {
    for (int j = 0; j < BMP_HEIGTH; j++) // And each column.
    {
      if (tmp_image[i][j] == 255) // If pixel is white.
      {
        pixels++; // Increment pixel counter.
      }
    }
  }
  return pixels; // Return counted pixels.
}

int otsu(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int t_opt;                              // Optimal intensity value
  int hist[256] = {0};                    // Histogram array
  int numpixels = BMP_WIDTH * BMP_HEIGTH; // Number of pixels

  for (int t = 0; t <= 255; t++) // For each intensity
  {
    for (int i = 0; i < BMP_WIDTH; i++) // and for each column
    {
      for (int j = 0; j < BMP_HEIGTH; j++) // and for each row
      {
        if (tmp_image[i][j] == t) // If said pixel has intensity t
        {
          hist[t]++; // Increment intensity in histogram
        }
      }
    }
  }

  double cdf[256] = {0}; // Commulative distribution functio

  cdf[0] = hist[0] / (numpixels); // First cdf value
  for (int t = 1; t <= 255; t++)  // For each intensity
  {
    cdf[t] = cdf[t - 1] + (double)(hist[t] / numpixels); // Add commulated distribution
  }

  double var[256] = {0}; // Interclass variance array

  for (int t = 0; t <= 255; t++)
  {
    double w0 = 0.1; // Foreground class probability
    double w1 = 0.1; // Background class probability
    for (int i = 0; i <= t - 1; i++)
    {
      w0 += cdf[i];
    }
    for (int i = t; t < 255; t++)
    {
      w1 += cdf[i];
    }
    double m0 = 0;
    double m1 = 0;
    for (int i = 0; i < t - 1; i++)
    {
      m0 += i * cdf[i];
    }
    m0 /= w0;
    for (int i = t; i < 255; i++)
    {
      m1 += i * cdf[i];
    }
    m1 /= w1;
    var[t] = w0 * w1 * pow(m0 - m1, 2);
  }
  double t = 0;
  for (int i = 0; i <= 255; i++)
  {
    if (var[i] > t)
    {
      t = var[i];
      t_opt = i;
    }
  }
  return t_opt;
}

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

  int t_opt;

  t_opt = otsu(tmp_image);
  printf("t_opt: %d\n", t_opt);
  // Apply binary threshold
  t_opt = 100;
  binary_threshold(tmp_image, t_opt);

  // Initialise counters for terminal writeout.
  int pixels = 0; // White pixels left in picture.
  int count = 0;  // Counted cells.
  int step = 0;   // Iteration steps.

  // Do-While loop for erosion and detection.
  do // Do the following:
  {
    pixels = pixelCheck(tmp_image);       // Count white pixels.
    printf("Pixels left: %d\n", pixels);  // Print counted whities.
    erode(tmp_image);                     // Perform erosion.
    count += detection(image, tmp_image); // Detect cells and add to counter.
    step++;                               // Increment step count.
    // to_rgb(tmp_image, image);
    // write_bitmap(image, argv[2]);
  } while (pixels != 0); // Until no whities are left.

  // Convert to output format for testing purposes
  printf("Steps: %d\n", step);
  printf("Cell Detection Program\n");
  printf("Total detected cells: %d\n", count);

  // Save image to file
  write_bitmap(image, argv[2]);

  printf("Done!\n");
  end = clock();
  cpu_time_used = (end - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("Total time: %.0f ms\n", cpu_time_used);
  return 0;
}
