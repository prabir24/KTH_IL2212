#ifndef _IMAGEPROCESSING_1_
#define _IMAGEPROCESSING_1_
#include "altera_avalon_mutex.h"
#include "altera_avalon_mutex_regs.h"

/*Specific location on shared on-chip memory for different processors for information sharing */
unsigned char** pros_1_start;
unsigned char** pros_1_dest;
unsigned char* pros_1_size;
unsigned char* pros_1_iteration;

unsigned char* pros_1_status;

/* get the mutex device handle */
alt_mutex_dev* mutex0;
alt_mutex_dev* mutex1;

/* function declarations */
void grayscale(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
void resize(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void correctness(unsigned char* src_image, unsigned char* dest_image, unsigned char size, unsigned char b_value[2]);
void sobelFilter(unsigned char* src_image, unsigned char* dest_image, unsigned char size);
//void toAsciiArt(unsigned char* src_image, unsigned char* dest_image, unsigned char size);

#endif
