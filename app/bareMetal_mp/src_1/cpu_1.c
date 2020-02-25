#include <stdio.h>
#include "system.h"
#include "imageProcessing.h"

#define TRUE 1

int main()
{
	unsigned char* b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4002);
	unsigned char* actual_b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4000);

	pros_1_status = (unsigned char*)(SHARED_ONCHIP_BASE+7200);
	*pros_1_status = 1;	

	/* get the mutex device handle */
	mutex0 = altera_avalon_mutex_open( "/dev/mutex_0" );
	mutex1 = altera_avalon_mutex_open( "/dev/mutex_1" );

	/*memory location for image on shared on-chip memory */
	pros_1_start = (unsigned char**)(SHARED_ONCHIP_BASE+7000);
	pros_1_dest = (unsigned char**)(SHARED_ONCHIP_BASE+7010);
	pros_1_size = (unsigned char*)(SHARED_ONCHIP_BASE+7020);
	pros_1_iteration = (unsigned char*)(SHARED_ONCHIP_BASE+7024);

	/* local variables */
	unsigned char iterations = *pros_1_iteration;
	unsigned char temp_1;
	
	while(altera_avalon_mutex_first_lock(mutex1) == 1);

	for(temp_1 = 0; temp_1 < iterations; temp_1++)
	{	
		altera_avalon_mutex_lock( mutex1, 1 );
		grayscale(*pros_1_start, *pros_1_dest, *pros_1_size);
		altera_avalon_mutex_unlock( mutex1);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex1, 1 );
		altera_avalon_mutex_unlock( mutex0);
		resize(*pros_1_start, *pros_1_dest, *pros_1_size, b_value);
		altera_avalon_mutex_unlock( mutex1);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex1, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		correctness(*pros_1_start, *pros_1_dest, *pros_1_size, actual_b_value);
		altera_avalon_mutex_unlock( mutex1);

		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex1, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		sobelFilter(*pros_1_start, *pros_1_dest, *pros_1_size);
		altera_avalon_mutex_unlock( mutex1);
		
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_unlock( mutex0);		
	}
	
	while (TRUE);

	return 0;
}
