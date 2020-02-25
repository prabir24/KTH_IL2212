#include <stdio.h>
#include "system.h"
#include "imageProcessing.h"

#define TRUE 1

int main()
{
	unsigned char* b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4004);
	unsigned char* actual_b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4000);

	pros_2_status = (unsigned char*)(SHARED_ONCHIP_BASE+7201);
	*pros_2_status = 1;

	/* get the mutex device handle */
	mutex0 = altera_avalon_mutex_open( "/dev/mutex_0" );
	mutex2 = altera_avalon_mutex_open( "/dev/mutex_2" );

	/*memory location for image on shared on-chip memory */
	pros_2_start = (unsigned char**)(SHARED_ONCHIP_BASE+7030);
	pros_2_dest = (unsigned char**)(SHARED_ONCHIP_BASE+7040);
	pros_2_size = (unsigned char*)(SHARED_ONCHIP_BASE+7050);
	pros_2_iteration = (unsigned char*)(SHARED_ONCHIP_BASE+7054);

	/* local variables */
	unsigned char iterations = *pros_2_iteration;
	unsigned char temp_1;
	
	while(altera_avalon_mutex_first_lock(mutex2) == 1);
		
	for(temp_1 = 0; temp_1 < iterations; temp_1++)
	{
		altera_avalon_mutex_lock( mutex2, 1 );
		grayscale(*pros_2_start, *pros_2_dest, *pros_2_size);
		altera_avalon_mutex_unlock( mutex2);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex2, 1 );
		altera_avalon_mutex_unlock( mutex0);
		resize(*pros_2_start, *pros_2_dest, *pros_2_size, b_value);
		altera_avalon_mutex_unlock( mutex2);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex2, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		correctness(*pros_2_start, *pros_2_dest, *pros_2_size, actual_b_value);
		altera_avalon_mutex_unlock( mutex2);

		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex2, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		sobelFilter(*pros_2_start, *pros_2_dest, *pros_2_size);
		altera_avalon_mutex_unlock( mutex2);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_unlock( mutex0);
	}
	
	while (TRUE);

	return 0;
}
