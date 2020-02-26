#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_performance_counter.h"
#include "io.h"
#include "imageProcessing.h"
#include "images/images_alt.h"

#define TRUE 1
#define PERFORMANCE_COUNT 32

unsigned char count;

int main()
{
	
#ifdef DEBUG
	count = 1;
#endif

#ifdef PERFORMANCE
	count = PERFORMANCE_COUNT;
#endif
	
	/* local variable */
	short temp_1 = 0, temp_2;
	unsigned char b_value[2];
	unsigned char *dest_image = NULL;
#ifdef DEBUG
	short temp_3;
	short length, lengthX;
#endif
#ifdef PERFORMANCE
	unsigned int total_execution_time;
	unsigned int execution_time_per_image;
	unsigned int throughput;
#endif

	/********************************************************************************/
	/********************** PERFORMANCE COUNTER INITIALIZATION **********************/
	/********************************************************************************/
	
#ifdef PERFORMANCE
	
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

	int timer_overhead_perf = 0;
      	for (temp_1 = 0; temp_1 < 10; temp_1++) 
	{      
        	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE,1);
        	PERF_END(PERFORMANCE_COUNTER_0_BASE,1);
        	timer_overhead_perf = timer_overhead_perf + perf_get_section_time((void*)PERFORMANCE_COUNTER_0_BASE, 1);
      	}

      	timer_overhead_perf = timer_overhead_perf / 10;
        
      	//printf("Performance Counter Overhead in ticks: %d\n", (int) timer_overhead_perf);
	//printf("Performance Counter Frequency : %d\n", (int) alt_get_cpu_freq());
      	//printf("Timer overhead in ms:    %f\n\n", 1000.0 * (float)timer_overhead_perf/(float)alt_get_cpu_freq());


	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);	
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);
	
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE,1);
#endif

	/********************************************************************************/
	/******************************* APPLICATION FUNCTIONS **************************/
	/********************************************************************************/
	for(temp_1 = 0; temp_1 < count; temp_1++)
	{
		for(temp_2 = 0; temp_2 < sequence1_length; temp_2++)
		{
			if(*sequence1[temp_2] != 32 || *(sequence1[temp_2]+1) != 32)
			{
				printf("Image is not 32*32 size\n");
				continue;
			}
			/* convert 32 * 32 24-bit image to grayscale image */ 
			dest_image = grayscale(sequence1[temp_2]);

			/* resize 32 * 32 image to 16*16 image */
			resize(dest_image, b_value);
			
			/* correct the pixel values based on the difference between max and min of image */
			correctness(dest_image, b_value);
			
			/* sobel-filter implementation */
			sobelFilter(dest_image);

			/*convert 256-values pixel to 16-values ascii characters */
			toAsciiArt(dest_image);
#ifdef DEBUG
			
			printf("\n\n ******Output Image****** \n");
			length = dest_image[0] * dest_image[1];
			lengthX = dest_image[0];
			dest_image = dest_image + 3;

			for(temp_3 = 0; temp_3 < length; temp_3++)
			{
				if((temp_3 % lengthX) == 0)
					printf("\n");
				printf("%c",dest_image[temp_3]);
			}
#endif
			dest_image = dest_image - 3;
			free(dest_image);
			dest_image = NULL;
		}
	}

	/********************************************************************************/
	/************************* PERFORMANCE COUNTER CALCULATION **********************/
	/********************************************************************************/
#ifdef PERFORMANCE	
	PERF_END(PERFORMANCE_COUNTER_0_BASE,1);
	PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);

	total_execution_time = ((unsigned int)perf_get_section_time((void*)PERFORMANCE_COUNTER_0_BASE, 1) - timer_overhead_perf)
				/((unsigned int)alt_get_cpu_freq()/1000);

	execution_time_per_image = total_execution_time / (count*sequence1_length);

	throughput = 1000/ execution_time_per_image;
	
	printf("\nPerformance Counter Frequency : %d\n", (unsigned int)alt_get_cpu_freq());
	printf("Performance Counter in ticks : %d\n", 
		(unsigned int) (perf_get_section_time((void*)PERFORMANCE_COUNTER_0_BASE, 1) - timer_overhead_perf));
	printf("Number of Images Executed : %d\n", (count*sequence1_length));
	printf("Total Execution Time : %d ms\n", total_execution_time);
	printf("Execution Time Per Image : %d ms\n", execution_time_per_image);
	printf("Throughput : %d (images/second)\n", throughput);
	
#endif
	while (TRUE);

	return 0;
}
