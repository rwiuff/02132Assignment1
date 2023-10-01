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

// Detection method
int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int count, int captureSize)
{
    for (int i = 0; i < BMP_WIDTH; i++) // Iterate over picture columns.
    {
        for (int j = 0; j < BMP_HEIGTH; j++) // Iterate over picture rows.
        {
            if (capture(tmp_image, i, j, captureSize) == 1) // Check if a clear frame contains a cell.
            {
                count++;                                                                                      // Increment cell counter.
                drawX(image, i + captureSize / 2, j + captureSize / 2);                                       // Draw a cross on the detected cell.
                erase(tmp_image, i, j, captureSize);                                                          // Clear the area of white pixels.
                printf("| Cell %3d found at (%3d,%3d) |\n", count, i + captureSize / 2, j + captureSize / 2); // Print to console
                i += captureSize / 2;                                                                         // Move along in this iteration
                j += captureSize / 2;
            }
        }
    }
    return count; // Return this iterations counted cells.
}

// Detection of pixels in capture area.
int capture(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize)
{
    int capture = 0;                               // Variable indicating contents in capture area.
    for (int k = i; k <= i + captureSize + 1; k++) // Iterate within cell area columns.
    {
        for (int l = j; l <= j + captureSize + 1; l++) // Iterate within cell area rows.
        {
            if (tmp_image[k][l] == 255) // If a white pixel exists.
            {
                if (k == i || k == i + captureSize + 1 || l == j || l == j + captureSize + 1) // If its in the exclusion zone
                {
                    capture = 0;    // Cell capture invalid
                    return capture; // Return invalid
                }
                else
                {
                    capture++; // Count cell pixel
                }
            }
        }
    }
    capture = (capture > 0) ? 1 : 0; // If cell pixels exist set capture to 1
    return capture;
}

// Draw crosses on cells
void drawX(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
    int thickness = 1;
    for (int dx = -5; dx <= 5; dx++)
    {
        for (int dy = -5; dy <= 5; dy++)
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

// Erase discovered cells
void erase(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize)
{
    for (int k = i; k <= i + captureSize; k++) // Iterate over cell area columns
    {
        for (int l = j; l <= j + captureSize; l++) // Iterate over cell area rows
        {
            tmp_image[k][l] = 0; // Zerorise pixel value.
        }
    }
}