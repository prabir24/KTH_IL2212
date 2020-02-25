#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_performance_counter.h"
#include "io.h"
#include "imageProcessing.h"
#include "images/images_alt.h"

#define TRUE 1
#define PERFORMANCE_COUNT 32

/* Global Variable for no of iterations */
unsigned char count;

int main()
{
	
#ifdef DEBUG
	count = 1;
#endif

#ifdef PERFORMANCE
	count = PERFORMANCE_COUNT;
#endif
	/* get the mutex device handle */
	mutex0 = altera_avalon_mutex_open( "/dev/mutex_0" );
	mutex1 = altera_avalon_mutex_open( "/dev/mutex_1" );
	mutex2 = altera_avalon_mutex_open( "/dev/mutex_2" );
	mutex3 = altera_avalon_mutex_open( "/dev/mutex_3" );
	mutex4 = altera_avalon_mutex_open( "/dev/mutex_4" );

	/* acquire the mutex, setting the value to one */
	altera_avalon_mutex_lock( mutex0, 1 );
	altera_avalon_mutex_lock( mutex1, 1 );
	altera_avalon_mutex_lock( mutex2, 1 );
	altera_avalon_mutex_lock( mutex3, 1 );
	altera_avalon_mutex_lock( mutex4, 1 );

	/* Memory allocated for CPU_1 to CPU_4 in Shared Memory for status*/ 
	statusMem((unsigned char*)SHARED_ONCHIP_BASE);

	/*memory location for image on shared on-chip memory*/

	unsigned char* image_loc;
	image_loc = (unsigned char*)SHARED_ONCHIP_BASE;

	unsigned char* grayscale_image_loc = image_loc+5000;
	unsigned char* resize_image_loc = image_loc;
	unsigned char* brightness_loc = image_loc+4000;
	unsigned char* correctness_image_loc = image_loc+5000;

	unsigned char* sobelFilter_image_loc = image_loc;
	unsigned char* asciiArt_loc = image_loc+5000;

	/* local variable */
	short temp_1 = 0, temp_2;
#ifdef DEBUG
	short temp_3;
	short length, lengthX;
#endif
#ifdef PERFORMANCE
	unsigned int total_execution_time;
	unsigned int execution_time_per_image;
	unsigned int throughput;
#endif

	/* Location in shared memory for inter Processor Communication */
	memory4Sharing(image_loc);

	/* Updating No. of iterations in shared memory that is to be read by CPU_1 to CPU_4 */ 
	iterations(count*sequence1_length);

	firstExecution = TRUE;

	/* wait for all processors to be synced */
	while(readStatus() == 0);

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
			/* copy image to shared on-chip memory */
			copyImage(sequence1[temp_2], image_loc);

			/* convert 32 * 32 24-bit image to grayscale image */ 
			grayscale(image_loc, grayscale_image_loc);

			/* resize 32 * 32 image to 16*16 image */
			resize(grayscale_image_loc, resize_image_loc);

			/* correct the pixel values based on the difference between max and min of image */
			correctness(resize_image_loc, correctness_image_loc, brightness_loc);
	
			/* sobel-filter implementation */
			sobelFilter(correctness_image_loc, sobelFilter_image_loc);

			/*convert 256-values pixel to 16-values ascii characters */
			toAsciiArt(sobelFilter_image_loc, asciiArt_loc);

#ifdef DEBUG
			
			printf("\n ******Output Image****** \n");
			length = asciiArt_loc[0] * asciiArt_loc[1];
			lengthX = asciiArt_loc[0];
			asciiArt_loc = asciiArt_loc + 3;

			for(temp_3 = 0; temp_3 < length; temp_3++)
			{
				if((temp_3 % lengthX) == 0)
					printf("\n");
				printf("%c",asciiArt_loc[temp_3]);
			}
#endif
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
