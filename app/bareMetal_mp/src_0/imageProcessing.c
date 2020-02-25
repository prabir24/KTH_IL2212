#include "imageProcessing.h"

char asciiLevels[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'};

/*Specific location on shared on-chip memory for different processors */
void memory4Sharing(unsigned char* image_loc)
{
	pros_1_start = (unsigned char**)(image_loc+7000);
	pros_1_dest = (unsigned char**)(image_loc+7010);
	pros_1_size = image_loc+7020;
	pros_1_iteration = image_loc+7024;

	pros_2_start = (unsigned char**)(image_loc+7030);
	pros_2_dest = (unsigned char**)(image_loc+7040);
	pros_2_size = image_loc+7050;
	pros_2_iteration = image_loc+7054;

	pros_3_start = (unsigned char**)(image_loc+7060);
	pros_3_dest = (unsigned char**)(image_loc+7070);
	pros_3_size = image_loc+7080;
	pros_3_iteration = image_loc+7084;
	
	pros_4_start = (unsigned char**)(image_loc+7090);
	pros_4_dest = (unsigned char**)(image_loc+7100);
	pros_4_size = image_loc+7110;
	pros_4_iteration = image_loc+7114;

}

void iterations(unsigned char val)
{
	*pros_1_iteration = val;
	*pros_2_iteration = val;
	*pros_3_iteration = val;
	*pros_4_iteration = val;
}

void statusMem(unsigned char* image_loc)
{
	pros_1_status = image_loc+7200;
	pros_2_status = image_loc+7201;
	pros_3_status = image_loc+7202;
	pros_4_status = image_loc+7203;
}

unsigned char readStatus()
{
	if ((*pros_1_status == 1) && (*pros_2_status == 1) && (*pros_3_status == 1) && (*pros_4_status == 1))
	{
		*pros_1_status = 0;
		*pros_2_status = 0;
		*pros_3_status = 0;
		*pros_4_status = 0;
		return 1;
	}
	else 
		return 0;
}

void copyImage(unsigned char* src_image, unsigned char* dest_image)
{
	short temp_1 = 0;
	short length = (src_image[0] * src_image[1] * 3) + 3;
		
	for(temp_1 = 0; temp_1 < length; temp_1++)
		dest_image[temp_1] = src_image[temp_1];	
}

unsigned char checkMutexStatus()
{
	int status1, status2, status3, status4;
	
	if ( (status1 = altera_avalon_mutex_trylock( mutex1, 1 )) == 0)
		altera_avalon_mutex_unlock( mutex1);
	if ( (status2 = altera_avalon_mutex_trylock( mutex2, 1 )) == 0)
		altera_avalon_mutex_unlock( mutex2);
	if ( (status3 = altera_avalon_mutex_trylock( mutex3, 1 )) == 0)
		altera_avalon_mutex_unlock( mutex3);
	if ( (status4 = altera_avalon_mutex_trylock( mutex4, 1 )) == 0)
		altera_avalon_mutex_unlock( mutex4);
	if( (status1 != 0) && (status2 != 0) && (status3 != 0) && (status4 != 0)) // means all mutex locked
	{
		return 1;
	}
	else if( (status1 == 0) && (status2 == 0) && (status3 == 0) && (status4 == 0))  // 0 means all mutex free
	{
		return 0;  	
	}
	else
		return 2;
}

void grayscale(unsigned char* src_image, unsigned char* dest_image)
{
	unsigned char temp_1 = 0, temp_3 = 0;
	short temp_2 = 0;
	short length, length1 ;
	
	temp_3 = 7;//(src_image[0] / 5) + 1;
	length = (src_image[0] - (temp_3 << 2)) << 5;//* src_image[1];
	length1 = temp_3 << 5;//* src_image[1];

	*pros_1_size = length1;
	*pros_2_size = length1;
	*pros_3_size = length1;
	*pros_4_size = length1;

	dest_image[0] = src_image[0];
	dest_image[1] = src_image[1];
	dest_image[2] = src_image[2];

	src_image = src_image+3;
	dest_image = dest_image+3;

	*pros_1_start = src_image + (length << 1) + length;
	*pros_1_dest = dest_image + length;
	
	*pros_2_start = *pros_1_start + (length1 << 1) + length1;
	*pros_2_dest = *pros_1_dest + length1;

	*pros_3_start = *pros_2_start + (length1 << 1) + length1;
	*pros_3_dest = *pros_2_dest + length1;

	*pros_4_start = *pros_3_start + (length1 << 1) + length1;
	*pros_4_dest = *pros_3_dest + length1;

	altera_avalon_mutex_unlock( mutex1 );
	altera_avalon_mutex_unlock( mutex2 );
	altera_avalon_mutex_unlock( mutex3 );
	altera_avalon_mutex_unlock( mutex4 );

	if(firstExecution == 0)
	{
		altera_avalon_mutex_unlock( mutex0 );
		while (checkMutexStatus() != 1);
		altera_avalon_mutex_lock( mutex0, 1 );
	}
	else
		firstExecution = 0;

	while(temp_1 < length)
	{
		temp_2 = (temp_1 << 1) + temp_1; //temp_2 = temp_1*3
		
		//src_image[temp_1] = ((int)image[temp_2]*0.3125) + ((int)image[temp_2+1]*0.5625) + ((int)image[temp_2+2]*0.125);
		dest_image[temp_1] = (src_image[temp_2] >> 2) + (src_image[temp_2] >> 4) 
				+ (src_image[temp_2+1] >> 1) + (src_image[temp_2+1] >> 4) 
				+ (src_image[temp_2+2] >> 3) ;
		temp_1++;
	}
}



void resize(unsigned char* src_image, unsigned char* dest_image)
{
	short temp_6;
	unsigned char lengthX = src_image[0];
	unsigned char lengthY = src_image[1];
	short length,new_length;

	dest_image[0] = src_image[0] >> 1;
	dest_image[1] = src_image[1] >> 1;
	dest_image[2] = 255;

	temp_6 = (lengthY >> 2);
	new_length = (temp_6  >> 1)* dest_image[0];
	length = temp_6 * lengthX;

	*pros_1_size = temp_6;
	*pros_2_size = temp_6;
	*pros_3_size = temp_6;
	*pros_4_size = temp_6;

	src_image = src_image+3;
	dest_image = dest_image+3;

	*pros_1_start = src_image;
	*pros_1_dest = dest_image;
	
	*pros_2_start = *pros_1_start + length;
	*pros_2_dest = *pros_1_dest + new_length;

	*pros_3_start = *pros_2_start + length;
	*pros_3_dest = *pros_2_dest + new_length;

	*pros_4_start = *pros_3_start + length;
	*pros_4_dest = *pros_3_dest + new_length;
	
	while (checkMutexStatus() != 0);
	altera_avalon_mutex_unlock( mutex0 );
	while (checkMutexStatus() != 1);
	altera_avalon_mutex_lock( mutex0, 1 );
}

void correctness(unsigned char* src_image, unsigned char *dest_image, unsigned char* b_value)
{
	unsigned char length;
	unsigned char var_1, var_2;
	dest_image[0] = src_image[0];
	dest_image[1] = src_image[1];
	dest_image[2] = 255;

	length = (dest_image[1] >> 2)*dest_image[0];

	*pros_1_size = length;
	*pros_2_size = length;
	*pros_3_size = length;
	*pros_4_size = length;

	src_image = src_image+3;
	dest_image = dest_image+3;

	*pros_1_start = src_image;
	*pros_1_dest = dest_image;
	
	*pros_2_start = *pros_1_start + length;
	*pros_2_dest = *pros_1_dest + length;

	*pros_3_start = *pros_2_start + length;
	*pros_3_dest = *pros_2_dest + length;

	*pros_4_start = *pros_3_start + length;
	*pros_4_dest = *pros_3_dest + length;

	while (checkMutexStatus() != 0);
	
	/* min value stored in b_value[0] */
	var_1 = (b_value[2] < b_value[4]) ?  b_value[2] : b_value[4];
	var_2 = (b_value[6] < b_value[8]) ?  b_value[6] : b_value[8];
	b_value[0] = (var_1 < var_2) ? var_1 : var_2;

	/* max value stored in b_value[1] */
	var_1 = (b_value[3] > b_value[5]) ?  b_value[3] : b_value[5];
	var_2 = (b_value[7] > b_value[9]) ?  b_value[7] : b_value[9];
	b_value[1] = (var_1 > var_2) ? var_1 : var_2;

	altera_avalon_mutex_unlock( mutex0 );
	while (checkMutexStatus() != 1);
	altera_avalon_mutex_lock( mutex0, 1 );
}

void sobelFilter(unsigned char* src_image, unsigned char* dest_image)
{
	unsigned char length_1, length_rest;
	unsigned char length_1_d, length_rest_d;
	short temp_1, temp_2, temp_3, temp_4, temp_5, temp_6 = 0;
	int sobel_Gx, sobel_Gy, sobel;

	dest_image[0] = src_image[0] - 2;
	dest_image[1] = src_image[1] - 2;
	dest_image[2] = 255;

	length_1 = src_image[0] << 1;
	length_rest = (src_image[0] << 1) + src_image[0];

	length_1_d = dest_image[0] << 1;
	length_rest_d = (dest_image[0] << 1) + dest_image[0];

	*pros_1_size = 3;
	*pros_2_size = 3;
	*pros_3_size = 3;
	*pros_4_size = 3;

	src_image = src_image+3;
	dest_image = dest_image+3;

	*pros_1_start = src_image + length_1;
	*pros_1_dest = dest_image + length_1_d;
	
	*pros_2_start = *pros_1_start + length_rest;
	*pros_2_dest = *pros_1_dest + length_rest_d;

	*pros_3_start = *pros_2_start + length_rest;
	*pros_3_dest = *pros_2_dest + length_rest_d;

	*pros_4_start = *pros_3_start + length_rest;
	*pros_4_dest = *pros_3_dest + length_rest_d;

	while (checkMutexStatus() != 0);
	altera_avalon_mutex_unlock( mutex0 );
	while (checkMutexStatus() != 1);
	altera_avalon_mutex_lock( mutex0, 1 );

	for(temp_1 = 0; temp_1 < 2; temp_1++)
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
	while (checkMutexStatus() != 0);
	altera_avalon_mutex_lock( mutex1, 1 );
	altera_avalon_mutex_lock( mutex2, 1 );
	altera_avalon_mutex_lock( mutex3, 1 );
	altera_avalon_mutex_lock( mutex4, 1 );
}

void toAsciiArt(unsigned char* src_image, unsigned char* dest_image)
{
	short temp_1 = 0;
	short final_length = src_image[0] * src_image[1];

	dest_image[0] = src_image[0];
	dest_image[1] = src_image[1];

	src_image = src_image+3;
	dest_image = dest_image+3;

	for(temp_1 = 0; temp_1 < final_length; temp_1++) 
	{
		dest_image[temp_1] = asciiLevels[src_image[temp_1]];
	}
}

