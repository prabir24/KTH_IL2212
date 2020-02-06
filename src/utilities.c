#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>


imageMatrix* rotateMatrix(imageMatrix* image, int X, int Y) 
{
	imageMatrix* new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
	new_image->image_values_d = (double*)malloc(sizeof(double) * image->lengthX * image->lengthY);
	new_image->lengthX = image->lengthX;
	new_image->lengthY = image->lengthY;

	int rotateX = X;
	int rotateY = Y;
	int lengthx = image->lengthX;
	int lengthy = image->lengthY;
	int temp_y;
	int temp_x;
	int temp_rotate;
	int rotate_flag = 0;

	if(abs(rotateX) > image->lengthX)
		rotateX = rotateX % lengthx;
	if(abs(rotateY) > image->lengthY)
		rotateY = rotateY % lengthy;

	/*rotate x-direction */
	for(temp_y = 0; temp_y < image->lengthY; temp_y++)
	{
		temp_rotate = 0;
		for(temp_x = 0; temp_x < image->lengthX; temp_x++)
		{		
			if(rotateX > 0 && (temp_x < (image->lengthX - rotateX)))
			{
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_y * image->lengthX) + temp_x + rotateX];
			}
			else if (rotateX > 0 && (temp_x >= (image->lengthX - rotateX)))
			{
				
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_y * image->lengthX) + temp_rotate];
				temp_rotate++;
			}
			else if(rotateX < 0 && (temp_x < abs(rotateX)))
			{
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_y * image->lengthX) + temp_x + image->lengthX + rotateX];
			}
			else if (rotateX < 0 && (temp_x >= abs(rotateX)))
			{
				
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_y * image->lengthX) + temp_rotate];
				temp_rotate++;
			}
		}
	}
	if(rotateX != 0)
	{
		image = new_image;
		new_image = (imageMatrix*)malloc(sizeof(imageMatrix));
		new_image->image_values_d = (double*)malloc(sizeof(double) * image->lengthX * image->lengthY);
		new_image->lengthX = image->lengthX;
		new_image->lengthY = image->lengthY;
	}

	/*rotate y-direction */
	temp_rotate = 0;
	for(temp_y = 0; temp_y < image->lengthY; temp_y++)
	{
		for(temp_x = 0; temp_x < image->lengthX; temp_x++)
		{		
			if(rotateY > 0 && (temp_y < (image->lengthY - rotateY)))
			{
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[((temp_y + rotateY) * image->lengthX) + temp_x ];
			}
			else if (rotateY > 0 && (temp_y >= (image->lengthY - rotateY)))
			{
				
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_rotate * image->lengthX) + temp_x ];
				rotate_flag = 1;
				printf("\n%f\n",image->image_values_d[0]);
			}
			else if(rotateY < 0 && (temp_y < abs(rotateY)))
			{
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[((temp_y + image->lengthY + rotateY) * image->lengthX) + temp_x ];
			}
			else if (rotateY < 0 && (temp_y >= abs(rotateY)))
			{
				
				new_image->image_values_d[(temp_y * image->lengthX) + temp_x] = image->image_values_d[(temp_rotate * image->lengthX) + temp_x ];
				rotate_flag = 1;
			}
		}
		if(rotate_flag == 1)
			temp_rotate++;
	}
	if(rotateY != 0)
		return new_image;
	else
		return image;

}

void main()
{
	double arr[5][3] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15}};
	imageMatrix x;
	x.image_values_d = &arr;
	x.lengthX = 3;
	x.lengthY = 5;
 	imageMatrix* y = rotateMatrix(&x, 1, 6);

	int i = 0,j = 0;

	for(i=0; i < 5; i++)
	{
		for(j=0; j < 3; j++)
		{
			printf("%f\t",y->image_values_d[(i*3) + j]);
		}
		printf("\n");
	}


}
