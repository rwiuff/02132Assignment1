#ifndef FUNCTIONS_H_

#define BMP_WIDTH 950
#define BMP_HEIGTH 950
#define BMP_CHANNELS 3

// Convert to grey scale
void grey_scale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH]);

// Binary threshold method.
void binary_threshold(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int t_opt);

// Erosion method
void erode(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH]);

// Count white pixels
int pixelCheck(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH]);

// Detection method
int detection(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int count, int captureSize);

// Detection of pixels in capture area.
int capture(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize);

// Draw crosses on cells
void drawX(unsigned char image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y);

// Erase discovered cells
void erase(unsigned char tmp_image[BMP_WIDTH][BMP_HEIGTH], int i, int j, int captureSize);

#endif