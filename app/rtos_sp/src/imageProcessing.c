#include "imageProcessing.h"

char asciiLevels[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'};

unsigned char* grayscale(unsigned char* src_image)
{
	short temp_1 = 0, temp_2 = 0, length;
	unsigned char* dest_image = NULL;

	length = src_image[0] * src_image[1];
	dest_image = (unsigned char*)malloc(length);

	dest_image[0] = src_image[0];
	dest_image[1] = src_image[1];
	dest_image[2] = src_image[2];
	src_image = src_image + 3;
 	dest_image = dest_image + 3;

	while(temp_1 < length)
	{
		temp_2 = (temp_1 << 1) + temp_1; //temp_2 = temp_1*3
		
		//src_image[temp_1] = ((int)image[temp_2]*0.3125) + ((int)image[temp_2+1]*0.5625) + ((int)image[temp_2+2]*0.125);
		dest_image[temp_1] = (src_image[temp_2] >> 2) + (src_image[temp_2] >> 4) 
				+ (src_image[temp_2+1] >> 1) + (src_image[temp_2+1] >> 4) 
				+ (src_image[temp_2+2] >> 3) ;
		temp_1++;
	}
	
	return (dest_image - 3);	
}



void resize(unsigned char* src_image, unsigned char* b_value)
{
	short temp_3, temp_4, temp_5 = 0;
	unsigned char temp_1, temp_2;
	unsigned char lengthX = src_image[0];
	unsigned char lengthY = src_image[1];

	src_image[0] = src_image[0] >> 1;
	src_image[1] = src_image[1] >> 1;

	src_image = src_image + 3;

	for(temp_1 = 0; temp_1 < lengthY; temp_1 += 2)
	{
		temp_3 = temp_1 << 5;
		temp_4 = temp_3 + lengthX;
		for(temp_2 = 0; temp_2 < lengthX; temp_2 += 2)
		{
			src_image[temp_5] = (unsigned char)(((short)src_image[temp_3 + temp_2] + (short)src_image[temp_3 + temp_2 + 1]
						+ (short)src_image[temp_4 + temp_2] + (short)src_image[temp_4 + temp_2 + 1]) >> 2);
			if(temp_5 == 0)
			{
				b_value[0] = src_image[0];
				b_value[1] = src_image[0];
			}
			/*else if (dest_image[temp_6] > b_value[1]) 
	           		b_value[1] = dest_image[temp_6];
			else if (dest_image[temp_6] < b_value[0])
				b_value[0] = dest_image[temp_6];*/

			(src_image[temp_5] > b_value[1]) ? (b_value[1] = src_image[temp_5]) : 
							((src_image[temp_5] < b_value[0]) ? (b_value[0] = src_image[temp_5]) : 0);

			temp_5++;
		}
	}
}

void correctness(unsigned char* src_image, unsigned char* b_value)
{
	short temp_1 = 0;
	unsigned char b_diff = b_value[1] - b_value[0];
	short length = src_image[0] * src_image[1];

	src_image = src_image + 3;

	for(temp_1 = 0; temp_1 < length; temp_1++)
	{
		if(b_diff > 63 && b_diff <= 127)
			src_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 1;
		else if(b_diff > 31 && b_diff <= 63)
			src_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 2;
		else if(b_diff > 15 && b_diff <= 31)
			src_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 3;
		else if(b_diff <= 15)
			src_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 4;
		else
			src_image[temp_1] =  src_image[temp_1];
	}
}

void sobelFilter(unsigned char* src_image)
{
	unsigned char lengthX;
	unsigned char lengthY;
	short temp_1, temp_2, temp_3, temp_4, temp_5, temp_6 = 0;
	int sobel_Gx, sobel_Gy, sobel;

	src_image[0] = src_image[0] - 2;
	src_image[1] = src_image[1] - 2;

	lengthX = src_image[0];
	lengthY = src_image[1];

	src_image = src_image+3;

	for(temp_1 = 0; temp_1 < lengthY; temp_1++)
	{
		temp_3 = temp_1 << 4;
		temp_4 = (temp_1+1) << 4;
		temp_5 = (temp_1+2) << 4;
		temp_6 = temp_1 * 14;

		for(temp_2 = 0; temp_2 < lengthX; temp_2++)
		{
			sobel_Gx = -(src_image[temp_3+temp_2]) + src_image[temp_3+temp_2+2] 
					- ((int)(src_image[temp_4+temp_2]) << 1) 
					+ ((int)(src_image[temp_4+temp_2+2]) << 1) 
					- src_image[temp_5+temp_2] + src_image[temp_5+temp_2+2];

			sobel_Gy = -(src_image[temp_3+temp_2]) - ((int)(src_image[temp_3+temp_2+1]) << 1) -(src_image[temp_3+temp_2+2])
					+ src_image[temp_5+temp_2] + ((int)(src_image[temp_5+temp_2+1]) << 1) + src_image[temp_5+temp_2+2];

			sobel = ((sobel_Gx < 0) ? ((~sobel_Gx)+1) : sobel_Gx) 
							+ ((sobel_Gy < 0) ? ((~sobel_Gy)+1) : sobel_Gy);
			src_image[temp_6+temp_2] = (unsigned char)(sobel >> 7);
		}
	}
}

void toAsciiArt(unsigned char* src_image)
{
	short temp_1 = 0;
	short final_length = src_image[0] * src_image[1];

	src_image = src_image+3;

	for(temp_1 = 0; temp_1 < final_length; temp_1++) 
	{
		src_image[temp_1] = asciiLevels[src_image[temp_1]];
	}
}

