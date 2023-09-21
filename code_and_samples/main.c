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

// Detection Feature

int inBounds(int x, int y)
{
  return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

void drawX(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
  int thickness = 1;
  for (int dx = -12; dx <= 12; dx++)
  {
    for (int dy = -12; dy <= 12; dy++)
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

int frameDetection(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  int clear = 1;
  int frameMask[52][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {1, 0}, {1, 13}, {2, 0}, {2, 13}, {3, 0}, {3, 13}, {4, 0}, {4, 13}, {5, 0}, {5, 13}, {6, 0}, {6, 13}, {7, 0}, {7, 13}, {8, 0}, {8, 13}, {9, 0}, {9, 13}, {10, 0}, {10, 13}, {11, 0}, {11, 13}, {12, 0}, {12, 13}, {13, 0}, {13, 1}, {13, 2}, {13, 3}, {13, 4}, {13, 5}, {13, 6}, {13, 7}, {13, 8}, {13, 9}, {13, 10}, {13, 11}, {13, 12}, {0, 13}};

  for (int x = 0; x < 52; x++)
  {
    // int varx = i + frameMask[x][0];
    // int vary = j + frameMask[x][1];
    if (tmp_image[i + frameMask[x][0]][j + frameMask[x][1]] == 255)
    {
      clear = 0;
      break;
    }
  }
  return clear;
}

int cellCapture(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  int detected = 0;

  for (int k = i + 1; k < i + 12; k++)
  {
    for (int l = j + 1; l < j + 12; l++)
    {
      if (tmp_image[k][l] == 255)
      {
        detected = 1;
      }
    }
  }
  return detected;
}

void erase(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j)
{
  for (int k = i; k <= i + 12; k++)
  {
    for (int l = j; l <= j + 12; l++)
    {
      tmp_image[k][l] = 0;
    }
  }
}

int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int count = 0;
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (frameDetection(tmp_image, i, j) == 0)
      {
        if (cellCapture(tmp_image, i, j) == 1)
        {
          count++;
          // printf("Drawin cell: %d\n", count);
          drawX(image, i + 7, j + 7);
          // printf("Erasing cell: %d\n", count);
          erase(tmp_image, i, j);
        }
      }
    }
  }
  return count;
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

void erode(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char ErosionMap[BMP_WIDTH][BMP_HEIGTH] = {0};
  int i;
  int j;
  for (i = 1; i <= (BMP_WIDTH - 1); i++)
  {
    tmp_image[i - 1][0] = 0;
    tmp_image[i - 1][BMP_HEIGTH] = 0;

    tmp_image[0][i - 1] = 0;
    tmp_image[BMP_WIDTH][i - 1] = 0;
    for (j = 1; j <= (BMP_HEIGTH - 1); j++)
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

int pixelCheck(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
  int pixels = 0;
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (tmp_image[i][j] == 255)
      {
        pixels++;
      }
    }
  }
  return pixels;
}

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

  start = clock();
  // Load image from file
  read_bitmap(argv[1], image);

  // Convert to grey scale
  grey_scale(image, tmp_image);

  // Apply binary threshold
  binary_threshold(tmp_image);

  // Erode the map to seperate cells
  // printf("PixelCheck: %d\n", pixelCheck(tmp_image));
  int pixels = 0;
  int count = 0;
  int step = 0;
  do
  {
    pixels = pixelCheck(tmp_image);
    printf("Pixels: %d\n", pixels);
    erode(tmp_image);
    count += detection(image, tmp_image);
    step++;
  } while (pixels != 0);

  // Convert to output format for testing purposes
  // to_rgb(tmp_image, output_image);
  printf("Steps: %d\n", step);
  printf("Cell Detection Program\n");
  printf("Total detected cells: %d\n", count);

  // Save image to file
  write_bitmap(image, argv[2]);

  printf("Done!\n");
  end = clock();
  cpu_time_used = (end - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("Total time: %f ms\n", cpu_time_used);
  return 0;
}
