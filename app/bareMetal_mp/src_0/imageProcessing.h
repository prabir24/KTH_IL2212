#ifndef _IMAGEPROCESSING_0_
#define _IMAGEPROCESSING_0_

#include "altera_avalon_mutex.h"

unsigned char firstExecution;

/*Specific location on shared on-chip memory for different processors for information sharing */

unsigned char** pros_1_start;
unsigned char** pros_1_dest;
unsigned char* pros_1_size;
unsigned char* pros_1_iteration;

unsigned char** pros_2_start;
unsigned char** pros_2_dest;
unsigned char* pros_2_size;
unsigned char* pros_2_iteration;

unsigned char** pros_3_start;
unsigned char** pros_3_dest;
unsigned char* pros_3_size;
unsigned char* pros_3_iteration;
	
unsigned char** pros_4_start;
unsigned char** pros_4_dest;
unsigned char* pros_4_size;
unsigned char* pros_4_iteration;

unsigned char* pros_1_status;
unsigned char* pros_2_status;
unsigned char* pros_3_status;
unsigned char* pros_4_status;

/* get the mutex device handle */
alt_mutex_dev* mutex0;
alt_mutex_dev* mutex1;
alt_mutex_dev* mutex2;
alt_mutex_dev* mutex3;
alt_mutex_dev* mutex4;

/* function declarations */
void memory4Sharing(unsigned char* src_image);
void statusMem(unsigned char* image_loc);
unsigned char readStatus();
void iterations(unsigned char val);

void copyImage(unsigned char* src_image, unsigned char* dest_image);

void grayscale(unsigned char* src_image, unsigned char* dest_image);
void resize(unsigned char* src_image, unsigned char* dest_image);
void correctness(unsigned char* src_image, unsigned char* dest_image, unsigned char* b_value);
void sobelFilter(unsigned char* src_image, unsigned char* dest_image);
void toAsciiArt(unsigned char* src_image, unsigned char* dest_image);

#endif
