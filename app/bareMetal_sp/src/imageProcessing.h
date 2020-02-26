#ifndef _IMAGEPROCESSING_
#define _IMAGEPROCESSING_
#include <stdlib.h>

/* function declarations */
unsigned char* grayscale(unsigned char* src_image);
void resize(unsigned char* src_image, unsigned char* b_value);
void correctness(unsigned char* src_image, unsigned char* b_value);
void sobelFilter(unsigned char* src_image);
void toAsciiArt(unsigned char* src_image);

#endif
