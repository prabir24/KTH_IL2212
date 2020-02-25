#ifndef _IMAGEPROCESSING_4_
#define _IMAGEPROCESSING_4_
#include "altera_avalon_mutex.h"

/*Specific location on shared on-chip memory for different processors for information sharing */
unsigned char** pros_4_start;
unsigned char** pros_4_dest;
unsigned char* pros_4_size;
unsigned char* pros_4_iteration;

unsigned char* pros_4_status;

/* get the mutex device handle */
alt_mutex_dev* mutex0;
alt_mutex_dev* mutex4;

/* function declarations */
void grayscale(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
void resize(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void correctness(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void sobelFilter(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
//void toAsciiArt(unsigned char* src_image, unsigned char* dest_image, unsigned char size);

#endif
