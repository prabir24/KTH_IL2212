#include "imageProcessing.h"

void grayscale(unsigned char* src_image, unsigned char* dest_image, unsigned char size)
{
	unsigned char temp_1 = 0;
	short temp_2 = 0;

	while(temp_1 < size)
	{
		temp_2 = ((short)temp_1 << 1) + temp_1; //temp_2 = temp_1*3
		
		//src_image[temp_1] = ((int)image[temp_2]*0.3125) + ((int)image[temp_2+1]*0.5625) + ((int)image[temp_2+2]*0.125);
		dest_image[temp_1] = (src_image[temp_2] >> 2) + (src_image[temp_2] >> 4) 
				+ (src_image[temp_2+1] >> 1) + (src_image[temp_2+1] >> 4) 
				+ (src_image[temp_2+2] >> 3) ;
		temp_1++;
	}
}

void resize(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2])
{
	short temp_3, temp_4, temp_5 = 0, temp_6;
	unsigned char temp_1, temp_2;
	unsigned char lengthX = 32;
	unsigned char lengthY = size;

	for(temp_1 = 0; temp_1 < lengthY; temp_1 += 2)
	{
		temp_3 = temp_1 << 5;
		temp_4 = temp_3 + lengthX;
		temp_5 = temp_1 << 3; // *16
		for(temp_2 = 0; temp_2 < lengthX; temp_2 += 2)
		{
			temp_6 = temp_5 + (temp_2 >> 1);
			
			dest_image[temp_6] = (unsigned char)(((short)src_image[temp_3 + temp_2] + (short)src_image[temp_3 + temp_2 + 1]
						+ (short)src_image[temp_4 + temp_2] + (short)src_image[temp_4 + temp_2 + 1]) >> 2);
			if(temp_6 == 0)
			{
				b_value[0] = dest_image[0];
				b_value[1] = dest_image[0];
			}
			/*else if (dest_image[temp_6] > b_value[1]) 
	           		b_value[1] = dest_image[temp_6];
			else if (dest_image[temp_6] < b_value[0])
				b_value[0] = dest_image[temp_6];*/

			(dest_image[temp_6] > b_value[1]) ? (b_value[1] = dest_image[temp_6]) : ((dest_image[temp_6] < b_value[0]) ? (b_value[0] = dest_image[temp_6]) : 0);
		}
	}
}


void correctness(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2])
{
	unsigned char temp_1 = 0;
	unsigned char b_diff = b_value[1] - b_value[0];

	for(temp_1 = 0; temp_1 < size; temp_1++)
	{
		if(b_diff > 63 && b_diff <= 127)
			dest_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 1;
		else if(b_diff > 31 && b_diff <= 63)
			dest_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 2;
		else if(b_diff > 15 && b_diff <= 31)
			dest_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 3;
		else if(b_diff <= 15)
			dest_image[temp_1] = ((src_image[temp_1] - b_value[0])) << 4;
		else
			dest_image[temp_1] =  src_image[temp_1];
	}
}

void sobelFilter(unsigned char* src_image, unsigned char* dest_image, unsigned char size)
{
	short temp_1, temp_2, temp_3, temp_4, temp_5, temp_6 = 0;
	int sobel_Gx, sobel_Gy, sobel;

	for(temp_1 = 0; temp_1 < size; temp_1++)
	{
		temp_3 = temp_1 << 4;
		temp_4 = (temp_1+1) << 4;
		temp_5 = (temp_1+2) << 4;
		temp_6 = temp_1 * 14;

		for(temp_2 = 0; temp_2 < 14; temp_2++)
		{
			sobel_Gx = -(src_image[temp_3+temp_2]) + src_image[temp_3+temp_2+2] 
					- ((int)(src_image[temp_4+temp_2]) << 1) 
					+ ((int)(src_image[temp_4+temp_2+2]) << 1) 
					- src_image[temp_5+temp_2] + src_image[temp_5+temp_2+2];

			sobel_Gy = -(src_image[temp_3+temp_2]) - ((int)(src_image[temp_3+temp_2+1]) << 1) -(src_image[temp_3+temp_2+2])
					+ src_image[temp_5+temp_2] + ((int)(src_image[temp_5+temp_2+1]) << 1) + src_image[temp_5+temp_2+2];

			sobel = ((sobel_Gx < 0) ? ((~sobel_Gx)+1) : sobel_Gx) 
							+ ((sobel_Gy < 0) ? ((~sobel_Gy)+1) : sobel_Gy);
			dest_image[temp_6+temp_2] = (unsigned char)(sobel >> 7); 
		}
	}
}

