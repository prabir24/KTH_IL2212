#include "imageProcessing.h"
#include <math.h>
#include <stdlib.h>

char asciiLevels[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'};

imageMatrix* grayscale(unsigned int* image, unsigned int lengthX, unsigned int lengthY)
{
	int temp_1 = 0, temp_2 = 0;
	int final_length = (lengthX / 3) * (lengthY);

	if(lengthX % 3 != 0)
	{
		printf("X length is not equal to 3");
		return NULL;
	}

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * final_length); 
	new_image->lengthX = (lengthX / 3);
 	new_image->lengthY = lengthY;

	while(temp_1 < final_length)
	{
		temp_2 = (temp_1 << 1) + temp_1; //temp_2 = temp_1*3
		
		new_image->image_values_d[temp_1] = (image[temp_2]*0.3125) + (image[temp_2+1]*0.5625) + (image[temp_2+2]*0.125);

		temp_1++;
	}

	free(image);
	return new_image;
}

imageMatrix* sumCols(imageMatrix* image, unsigned int factor)
{
	unsigned int temp_1 = 0, temp_2 = 0;
	unsigned int new_lengthX = image->lengthX / factor;
	unsigned int new_length = new_lengthX*image->lengthY;

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * new_length);
	//double* new_image = (double*)malloc(new_length*sizeof(double));
	new_image->lengthX = new_lengthX;
	new_image->lengthY = image->lengthY;

	while(temp_1 < new_length)
	{
		temp_2 = temp_1*factor;
		new_image->image_values_d[temp_1] = image->image_values_d[temp_2] + image->image_values_d[temp_2+1];
		temp_1++;
	}
	free(image);
	return new_image;	
}

imageMatrix* sumRows(imageMatrix* image, unsigned int factor)
{
	unsigned int temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0;
	unsigned int new_lengthY = image->lengthY / factor;
	unsigned int new_length = image->lengthX*new_lengthY;

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * new_length);
	new_image->lengthX = image->lengthX;
	new_image->lengthY = new_lengthY;
	//double* new_image = (double*)malloc(new_length*sizeof(double));

	for(temp_1 = 0; temp_1 < new_lengthY; temp_1++)
	{
		temp_2 = temp_1 * factor * image->lengthX;
		temp_3 = 0;
		temp_4 = temp_1*image->lengthX;
		while( temp_3 < image->lengthX )
		{
			
			new_image->image_values_d[temp_4+temp_3] = image->image_values_d[temp_2+temp_3] + image->image_values_d[temp_2+temp_3+image->lengthX];
			temp_3++;
		}
	}
	free(image);
	return new_image;		
}

imageMatrix* resize(imageMatrix* image)
{
	int temp = 0;
	imageMatrix* new_imageX = NULL;
	imageMatrix* new_imageY = NULL;

	new_imageX = sumCols(image, 2);
	new_imageY = sumRows(new_imageX, 2);
	
	unsigned int final_length = new_imageY->lengthX * new_imageY->lengthY;

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * final_length);
	new_image->lengthX = new_imageY->lengthX;
	new_image->lengthY = new_imageY->lengthY;

	 

	for(temp = 0; temp <= final_length; temp++)
	{
		new_image->image_values_d[temp] = new_imageY->image_values_d[temp];
		printf("%f\t", new_image->image_values_d[temp]);

	}
	printf("\n Size - %d", final_length);
	free(new_imageX);
	free(new_imageY);
	printf("hh\n");
	return new_image;
}

double* brightness(imageMatrix* image)
{
	double* brightness_value = (double*)malloc(2*sizeof(double));
	
	int temp;
    	brightness_value[0] = image->image_values_d[0];
	brightness_value[1] = image->image_values_d[0];

  	for (temp = 1; temp <= (image->lengthX * image->lengthY); temp++)
	{ 
        	if (image->image_values_d[temp] > brightness_value[1]) 
           		brightness_value[1] = image->image_values_d[temp];
		else if (image->image_values_d[temp] < brightness_value[0])
			brightness_value[0] = image->image_values_d[temp];
	}
	return brightness_value;
}

imageMatrix* sobelFilter_Gx(imageMatrix* image)
{
	int temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0, temp_5 = 0, temp_6 = 0;
	unsigned int final_length = (image->lengthX - 2)* (image->lengthY - 2);
	
	printf("Start\n");
	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * final_length);
	new_image->lengthX = image->lengthX - 2;
	new_image->lengthY = image->lengthY - 2;
	printf("%d %d\n",new_image->lengthX,new_image->lengthY);


	for(temp_1 = 0; temp_1 < new_image->lengthY; temp_1++)
	{
		temp_3 = temp_1 * image->lengthX;
		temp_4 = (temp_1+1) * image->lengthX;
		temp_5 = (temp_1+2) * image->lengthX;
		temp_6 = temp_1 * new_image->lengthX;

		for(temp_2 = 0; temp_2 < new_image->lengthX; temp_2++)
		{
			new_image->image_values_d[temp_6+temp_2] = - (image->image_values_d[temp_3+temp_2]) + image->image_values_d[temp_3+temp_2+2] - (image->image_values_d[temp_4+temp_2] * 2) + (image->image_values_d[temp_4+temp_2+2] * 2) - image->image_values_d[temp_5+temp_2] + image->image_values_d[temp_5+temp_2+2];
		}
	}

	printf("Done\n");
	return new_image;
}

imageMatrix* sobelFilter_Gy(imageMatrix* image)
{
	int temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0, temp_5 = 0, temp_6 = 0;
	unsigned int final_length = (image->lengthX - 2)* (image->lengthY - 2);

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * final_length);
	new_image->lengthX = image->lengthX - 2;
	new_image->lengthY = image->lengthY - 2;



	for(temp_1 = 0; temp_1 < new_image->lengthY; temp_1++)
	{
		temp_3 = temp_1 * image->lengthX;
		temp_4 = (temp_1+1) * image->lengthX;
		temp_5 = (temp_1+2) * image->lengthX;
		temp_6 = temp_1 * new_image->lengthX;

		for(temp_2 = 0; temp_2 < new_image->lengthX; temp_2++)
		{
			new_image->image_values_d[temp_6+temp_2] = - (image->image_values_d[temp_3+temp_2]) - (image->image_values_d[temp_3+temp_2+1] * 2) - image->image_values_d[temp_3+temp_2+2] + image->image_values_d[temp_5+temp_2] + (image->image_values_d[temp_5+temp_2+1] * 2) + image->image_values_d[temp_5+temp_2+2];
		}
	}

	return new_image;
}

imageMatrix* sobelFilter(imageMatrix* image)
{
	int temp_1 = 0;
	imageMatrix* Gx = sobelFilter_Gx(image);
	imageMatrix* Gy = sobelFilter_Gy(image);

	unsigned int final_length = (Gy->lengthX)* (Gy->lengthY);

	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * final_length);
	new_image->lengthX = Gy->lengthX;
	new_image->lengthY = Gy->lengthY;

	for(temp_1 = 0; temp_1 < final_length; temp_1++)
	{
		new_image->image_values_d[temp_1] = sqrt((Gx->image_values_d[temp_1] * Gx->image_values_d[temp_1]) + (Gy->image_values_d[temp_1] * Gy->image_values_d[temp_1]));
	}

	free(Gx);
	free(Gy);
	free(image);
	return new_image;
}

char* toAsciiArt(imageMatrix* image)
{
	unsigned int temp_1 = 0;
	unsigned int final_length = image->lengthX * image->lengthY;
	char * final_ascii_image = (char*)malloc(sizeof(char)*((image->lengthX*image->lengthY)+1));
	

	for(temp_1 = 0; temp_1 < final_length; temp_1++) 
	{
		final_ascii_image[temp_1] = asciiLevels[15 * (int)(image->image_values_d[temp_1]/255.0)];
	}

	final_ascii_image[temp_1] = '\0';

	return final_ascii_image;
}

