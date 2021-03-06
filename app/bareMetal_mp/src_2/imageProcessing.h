#ifndef _IMAGEPROCESSING_2_
#define _IMAGEPROCESSING_2_
#include "altera_avalon_mutex.h"

/*Specific location on shared on-chip memory for different processors for information sharing */
unsigned char** pros_2_start;
unsigned char** pros_2_dest;
unsigned char* pros_2_size;
unsigned char* pros_2_iteration;

unsigned char* pros_2_status;

/* get the mutex device handle */
alt_mutex_dev* mutex0;
alt_mutex_dev* mutex2;

/* function declarations */
void grayscale(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
void resize(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void correctness(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void sobelFilter(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
//void toAsciiArt(unsigned char* src_image, unsigned char* dest_image, unsigned char size);

#endif
