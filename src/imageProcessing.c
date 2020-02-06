#include "imageProcessing.h"

double* grayscale(unsigned int* image, unsigned int lengthX, unsigned int lengthY)
{
	int temp_Y = 0;
	double* new_image = (double*)malloc(lengthY*sizeof(double)); 
	if(lengthX != 3)
	{
		printf("X length is not equal to 3");
		return 0.0;
	}
	while(temp_Y <= lengthY)
	{
		new_image[temp_Y] = (image[temp_Y]*0.3125) + (image[temp_Y+1]*0.5625) + (image[temp_Y+2]*0.125);
		temp_Y = temp_Y + lengthX;
	}
	return new_image;
}

double* sumCols(double* image, unsigned int lengthX, unsigned int lengthY)
{
	int temp_Y = 0;
	double* new_image = (double*)malloc(lengthY*sizeof(double));

	while(temp_Y <= lengthY)
	{
		new_image[temp_Y] = image[temp_Y] + image[temp_Y+1];
		temp_Y = temp_Y + lengthX;
	}
	return new_image;	
}

double* sumRows(double* image, unsigned int lengthX, unsigned int lengthY)
{
	int temp_Y = 0;
	double* new_image = (double*)malloc(((lengthY/2)+1)*sizeof(double));

	while(temp_Y <= ((lengthY/2)+1))
	{
		new_image[temp_Y] = image[temp_Y] + image[temp_Y+1];
		temp_Y = temp_Y + 2;
	}
	return new_image;	
}

double* resize(double* image, unsigned int lengthX, unsigned int lengthY)
{
	int temp = 0;
	sumCols();
	sumRows()

	for(temp = 0; temp <= lengthY; temp++)
	{
		image[temp] = image[temp]/4;

	}

}

double* brightness(double* image, unsigned int lengthX, unsigned int lengthY)
{
	double* brightness_value = (double*)malloc(2*sizeof(double));
	
	int temp;
    	brightness_value[0] = image[0];
	brightness_value[1] = image[0];

  	for (temp = 1; temp <= (lengthX*lengthY); temp++)
	{ 
        	if (image[temp] > brightness_value[1]) 
           		brightness_value[1] = image[temp];
		else if (image[temp] < brightness_value[0])
			brightness_value[0] = image[temp];
	}
	return brightness_value;
}

