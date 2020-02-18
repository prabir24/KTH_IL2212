#include <stdio.h>
#include "utilities.h"

imageMatrix* grayscale(unsigned int* image, unsigned int lengthX, unsigned int lengthY);
imageMatrix* sumCols(imageMatrix* image, unsigned int factor);
imageMatrix* sumRows(imageMatrix* image, unsigned int factor);
imageMatrix* resize(imageMatrix* image);
double* brightness(imageMatrix* image);
imageMatrix* sobelFilter_Gx(imageMatrix* image);
imageMatrix* sobelFilter_Gy(imageMatrix* image);
imageMatrix* sobelFilter(imageMatrix* image);
char* toAsciiArt(imageMatrix* image);

