#include "functions.h"
#include "cbmp.h"
#include <stdio.h>

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

// Binary threshold method.
void binary_threshold(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int t_opt)
{
    // int threshold = 90;                  // Initial threshold.
    for (int i = 0; i <= BMP_WIDTH; i++) // Iterate over columns.
    {
        for (int j = 0; j <= BMP_HEIGTH; j++) // Iterate over rows.
        {
            tmp_image[i][j] = tmp_image[i][j] <= t_opt ? 0 : 255; // Ternary operator: If a pixel intensity is below or equal to
        }                                                         // threshold, set it to zero, otherwise 255
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

// Check for array overflow
int inBounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

// Detection method
int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])
{
    int count = 0;
    for (int i = 0; i < BMP_WIDTH; i++) // Iterate over picture columns.
    {
        for (int j = 0; j < BMP_HEIGTH; j++) // Iterate over picture rows.
        {
           if(image[i][j][0] == 255) {
                count++;
                drawX(image, i, j);
                printf("cell %d found at (%d, %d)", count, i, j);
           }
        }
    }
    return count; // Return this iterations counted cells.
}


// Draw crosses on cells
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