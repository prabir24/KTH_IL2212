#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imageProcessing.h"

#define FILE_TYPE ".ppm"

unsigned int string_size(char* filename)
{
	unsigned int size = -1;
	while(filename[++size] != '\0');
	return size;
}

unsigned int sub_string(char* filename, unsigned int size)
{
	char *file_type = FILE_TYPE;
	unsigned int cntr = size - string_size(file_type);
	unsigned int temp = 0;
	if(cntr < 1)
		return 0;
	while(cntr != size )
	{
		if(filename[cntr] != file_type[temp])
			break;
		else 
		{
			cntr++;
			temp++;
		}
	}
	if(cntr == size)
		return 1;
	else
		return 0;		
	
}

ppmFile* read_file(char *filename)
{
	char ch[16];
	int temp_1 = 0, temp_2 = 0, temp_3 = 0;
	FILE* file_ptr = NULL;
	file_ptr = fopen(filename, "r");
	if(file_ptr == NULL)
	{
		printf("Failed to open file: %s\n",filename);
		return NULL; 
	}
	/*while((ch = fgetc(file_ptr)) != EOF) 
	{
		printf("%d\t", (int)ch);
	}*/
	ppmFile* file = (ppmFile*)malloc(sizeof(ppmFile));
	if(fgets(ch, 16, file_ptr) != NULL)
	{
		fgets(ch, 16, file_ptr);

		while(ch[temp_1] != ' ')
			temp_1++;

		for(temp_2 = temp_1 -1; temp_2 >= 0; temp_2--)
			file->lengthX += pow(10, (temp_1-1-temp_2))*((int)ch[temp_2]-48);

		file->lengthX *= 3;

		temp_3 = temp_1;

		while(ch[temp_1] != '\0')
			temp_1++;

		for(temp_2 = temp_1 -2; temp_2 > temp_3; temp_2--)
			file->lengthY += pow(10, (temp_1-2-temp_2))*((int)ch[temp_2]-48);

		//printf("\n%d %d\n",file->lengthX,file->lengthY);
		   
	}
	fgets(ch, 16, file_ptr);
	file->pixelValues = (unsigned int*)malloc(sizeof(unsigned int) * file->lengthY * file->lengthX * 3);
	temp_1 = 0;
	while(fgets(ch, 16, file_ptr) != NULL)
	{
		file->pixelValues[temp_1] = (unsigned int)atoi(ch);
		temp_1++;
		//fgets(ch, 16, file_ptr)) 
		//printf("%d\t", ch);
	}
	/*printf("\nPixel Values :-\n");
	for(temp_1 = 0; temp_1 < (file->lengthY * file->lengthX * 3); temp_1++)
		printf("%d\t",file->pixelValues[temp_1]);
	printf("\n");*/
	fclose(file_ptr);
	return file;
}

void main(int argc, char *argv[])
{
	ppmFile* file = NULL;
	imageMatrix* image = NULL;
	imageMatrix* image1 = NULL;
	imageMatrix* image2 = NULL;
	char* asciiImage = NULL;
	int temp_1 = 0;

	if(argc < 2)
	{
		printf(" less arguments\n");
		return;	
	}
	else if(argc > 2)
	{
		printf(" more arguments\n");
		return;
	}

	struct dirent *dir_entry = NULL;

	DIR* dir = opendir(argv[1]);
	
	if(dir == NULL)
	{
		printf("Failed to open directory\n");
		return;
	}
	while((dir_entry = readdir(dir)) != NULL)
	{
		//printf("%s\n", dir_entry->d_name);
		if(sub_string(dir_entry->d_name, string_size(dir_entry->d_name)) == 1)
		{
			file = read_file(dir_entry->d_name);
			printf("\nSize after File Read - %d %d\n",file->lengthX, file->lengthY);

			image = grayscale(file->pixelValues, file->lengthX, file->lengthY);
			
			printf("\nSize after GrayScale - %d %d\n",image->lengthX, image->lengthY);
			//image1 = resize(image);
			//printf("\nSize after Resize - %d %d\n",image1->lengthX, image1->lengthY);

			for(temp_1 = 0; temp_1 < (image->lengthX * image->lengthY); temp_1++)
			{
				if((temp_1 % image->lengthX) == 0)
					printf("\n\n");
				printf("%f\t", image->image_values_d[temp_1]);
			}

			printf("Resize done\n");
			image2 = sobelFilter(image);

			for(temp_1 = 0; temp_1 < (image2->lengthX * image2->lengthY); temp_1++)
			{
				if((temp_1 % image2->lengthX) == 0)
					printf("\n\n");
				printf("%f\t", image2->image_values_d[temp_1]);
			}

			asciiImage = toAsciiArt(image2);

			printf("\n\n\nIMAGE\n");
			for(temp_1 = 0; temp_1 < (image2->lengthX * image2->lengthY); temp_1++)
			{
				if((temp_1 % image2->lengthX) == 0)
					printf("\n");
				printf("%c", asciiImage[temp_1]);
			}
			printf("\n");
			printf("Done\n");
			/*while(asciiImage[temp_1] != '\0')
			{
				printf("%
			}*/
		}
	}
	closedir(dir);

	

}

