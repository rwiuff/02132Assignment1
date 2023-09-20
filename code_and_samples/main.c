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


//Detection Feature

int inBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

void dfs(int x, int y, unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int visited[BMP_WIDTH][BMP_HEIGTH], int *sumX, int *sumY, int *total) {
    if (!inBounds(x, y) || visited[x][y] || tmp_image[x][y] != 255) {
        return;
    }
    visited[x][y] = 1;
    *sumX += x;
    *sumY += y;
    *total += 1;

    dfs(x + 1, y, tmp_image, visited, sumX, sumY, total);
    dfs(x - 1, y, tmp_image, visited, sumX, sumY, total);
    dfs(x, y + 1, tmp_image, visited, sumX, sumY, total);
    dfs(x, y - 1, tmp_image, visited, sumX, sumY, total);
}

int detection(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH]) {
    int i, j;
    int count = 0;
    
    int visited[BMP_WIDTH][BMP_HEIGTH] = { {0} };
    
    for (i = 0; i < BMP_WIDTH; i++) {
        for (j = 0; j < BMP_HEIGTH; j++) {
            if (tmp_image[i][j] == 255 && !visited[i][j]) {
                count++;
                
                int sumX = 0;
                int sumY = 0;
                int total = 0;
                dfs(i, j, tmp_image, visited, &sumX, &sumY, &total);
                
                int centroidX = sumX / total;
                int centroidY = sumY / total;

                int thickness = 1;
                for (int dx = -12; dx <= 12; dx++) {
                    for (int dy = -12; dy <= 12; dy++) {
                        if (inBounds(centroidX + dx, centroidY + dy)) {
                            if ((dx >= -thickness && dx <= thickness) || (dy >= -thickness && dy <= thickness)) {
                                output_image[centroidX + dx][centroidY + dy][0] = 255;
                                output_image[centroidX + dx][centroidY + dy][1] = 0;
                                output_image[centroidX + dx][centroidY + dy][2] = 0;
                            }
                        }
                    }
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

  
  start = clock();
  // Load image from file
  read_bitmap(argv[1], input_image);

  // Convert to grey scale
  grey_scale(input_image, tmp_image);

  // Apply binary threshold
  binary_threshold(tmp_image);

  // Convert to output format for testing purposes
  to_rgb(tmp_image, output_image);

  printf("Cell Detection Program\n");
  int count = detection(output_image, tmp_image);
  printf("Total detected cells: %d\n", count);

  // Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  end = clock();
  cpu_time_used = (end - start)*1000.0/CLOCKS_PER_SEC;
  printf("Total time: %f ms\n", cpu_time_used);
  return 0;
}
