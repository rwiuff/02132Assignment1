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
unsigned char erode(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH])

{
	unsigned char Change=0;
    unsigned char ErosionMap[BMP_WIDTH][BMP_HEIGTH] = {0}; // tmp_image sized matrix of 0's
    for (int i = 1; i <= (BMP_WIDTH - 1); i++) 
    {
		// Cut off Border to ensure erosion and fringe cases
        tmp_image[i - 1][0] = 0;
        tmp_image[i - 1][BMP_HEIGTH - 1] = 0;

        tmp_image[0][i - 1] = 0;
        tmp_image[BMP_WIDTH][i - 1] = 0;
        for (int j = 1; j <= (BMP_HEIGTH - 1); j++)
        {
            // Iterating over every pixel, if the sum of the surrounding area is not greater than 3x255 (i.e there's a black tile around) Then draw it up for erosion in the Erosion filter map 
            if ((tmp_image[i - 1][j] + tmp_image[i + 1][j] + tmp_image[i][j + 1] + tmp_image[i][j - 1] )<900)
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
				if (tmp_image[o][p] == 255){
					
					// Since it's cheaper to do all the erosions at once, we need to preserve the final pixels, so we check in a 2x2 area around if there's any connective pieces
					
					
					if ((((tmp_image[o - 1][p] == 0 && tmp_image[o + 1][p] == 0) && (tmp_image[o][p + 1] == 0 && tmp_image[o][p - 1] == 0)) 
					&& ((tmp_image[o - 1][p+1] == 0 && tmp_image[o + 1][p+1] == 0) && (tmp_image[o - 1][p-1] == 0 && tmp_image[o + 1][p-1] == 0)))&&
					(((tmp_image[o - 2][p] == 0 && tmp_image[o + 2][p] == 0) && (tmp_image[o][p + 2] == 0 && tmp_image[o][p - 2] == 0)) 
					&& ((tmp_image[o - 2][p+1] == 0 && tmp_image[o + 2][p+1] == 0) && (tmp_image[o - 2][p-1] == 0 && tmp_image[o + 2][p-1] == 0)))
					&&
					(((tmp_image[o - 2][p+2] == 0 && tmp_image[o + 2][p+2] == 0) && (tmp_image[o+1][p + 2] == 0 && tmp_image[o+1][p - 2] == 0)) 
					&& ((tmp_image[o - 1][p+2] == 0 && tmp_image[o + 2][p-2] == 0) && (tmp_image[o - 2][p-2] == 0 && tmp_image[o - 1][p-2] == 0)))
					
					)
					{
					tmp_image[o][p] = 255;
					}
					
					else
					{
					// This only gets triggered if There was a Pixel at any point up for Erosion, and it is fine to do so following the surrounding pixel criteria, 
					//so we use it to check if there was a change and whether to stop erosion
					  Change=1;
					  tmp_image[o][p] = 0;
					 }
					
					}
				}
            }
        }
    return Change;
}

// Check for array overflow
int inBounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < BMP_WIDTH && y < BMP_HEIGTH;
}

// Detection method
int detection(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    int count = 0;
    for (int i = 0; i < BMP_WIDTH; i++) // Iterate over picture columns.
    {
        for (int j = 0; j < BMP_HEIGTH; j++) // Iterate over picture rows.
        {
           if(tmp_image[i][j] == 255) {
                count++;
                drawX(image, i, j);
                printf("| cell %3d found at (%3d, %3d)|\n", count, i, j);
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
