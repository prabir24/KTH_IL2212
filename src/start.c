#include <stdio.h>
#include <dirent.h>
#include <string.h>

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

void read_file(char *filename)
{
	char ch;
	FILE* file_ptr = NULL;
	file_ptr = fopen(filename, "r");
	if(file_ptr == NULL)
	{
		printf("Failed to open file: %s\n",filename);
		return; 
	}
	while((ch = fgetc(file_ptr)) != EOF) 
	{
		printf("%d\t", (int)ch);
	}
	fclose(file_ptr);
}

void main(int argc, char *argv[])
{
	//FILE* file_ptr = NULL;
	//file_ptr = fopen(
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
		printf("%s\n", dir_entry->d_name);
		if(sub_string(dir_entry->d_name, string_size(dir_entry->d_name)) == 1)
		{
			read_file(dir_entry->d_name);
		}
	}
	closedir(dir);

}

