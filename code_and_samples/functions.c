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

// Check for array overflow
int inBounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

// Detection method
int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int count, int captureSize)
{
    for (int i = 0; i < BMP_WIDTH; i++) // Iterate over picture columns.
    {
        for (int j = 0; j < BMP_HEIGTH; j++) // Iterate over picture rows.
        {
            if (frameDetection(tmp_image, i, j, captureSize) != 1) // Check if a given frame is clear.
            {
                if (cellCapture(tmp_image, i, j, captureSize) == 1) // Check if a clear frame contains a cell.
                {
                    count++;                             // Increment cell counter.
                    drawX(image, i + 7, j + 7);          // Draw a cross on the detected cell.
                    erase(tmp_image, i, j, captureSize); // Clear the area of white pixels.
                    printf("| Cell %3d found at (%3d,%3d) |\n", count, i, j);
                }
            }
        }
    }
    return count; // Return this iterations counted cells.
}

// Detection of pixels in detection frame
int frameDetection(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize)
{
    int clear = 0;                                 // Variable indicating the frame is clear.
    for (int x = i; x <= i + captureSize + 1; x++) // Iterate over frame columns
    {
        if (tmp_image[x][j] == 255 || tmp_image[x][j + captureSize + 1] == 255) // Check first and last column
        {
            clear++;
            break;
        }
    }
    for (int x = j; x <= j + captureSize + 1; x++) // Iterate over frame rows
    {
        if (tmp_image[i][x] == 255 || tmp_image[i + captureSize + 1][x] == 255) // Check first and last row
        {
            clear++;
            break;
        }
    }
    clear = (clear >= 1) ? 1 : 0; // Convert detections into 1 for white pixels detected or 0
    return clear;
}

// Detection of pixels in capture area.
int cellCapture(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize)
{
    int detected = 0; // Variable indicating contents in capture area.

    for (int k = i + 1; k <= i + captureSize; k++) // Iterate within cell area columns.
    {
        for (int l = j + 1; l <= j + captureSize; l++) // Iterate within cell area rows.
        {
            if (tmp_image[k][l] == 255) // If a white pixel exists.
            {
                detected = 1; // Set contents flag.
            }
        }
    }
    return detected;
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

// Erase discovered cells
void erase(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize)
{
    for (int k = i; k <= i + captureSize; k++) // Iterate over cell area columns
    {
        for (int l = j; l <= j + captureSize; l++) // Iterate over cell area rows
        {
            if (inBounds(k, l) == 1)
            {
                tmp_image[k][l] = 0; // Zerorise pixel value.
            }
        }
    }
}