#include <stdio.h>
#include "system.h"
#include "imageProcessing.h"

#define TRUE 1

int main()
{
	unsigned char* b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4006);
	unsigned char* actual_b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4000);

	pros_3_status = (unsigned char*)(SHARED_ONCHIP_BASE+7202);
	*pros_3_status = 1;	
	
	/* get the mutex device handle */
	mutex0 = altera_avalon_mutex_open( "/dev/mutex_0" );
	mutex3 = altera_avalon_mutex_open( "/dev/mutex_3" );

	/*memory location for image on shared on-chip memory */
	pros_3_start = (unsigned char**)(SHARED_ONCHIP_BASE+7060);
	pros_3_dest = (unsigned char**)(SHARED_ONCHIP_BASE+7070);
	pros_3_size = (unsigned char*)(SHARED_ONCHIP_BASE+7080);
	pros_3_iteration = (unsigned char*)(SHARED_ONCHIP_BASE+7084);

	/* local variables */
	unsigned char iterations = *pros_3_iteration;
	unsigned char temp_1;
	
	while(altera_avalon_mutex_first_lock(mutex3) == 1);

	for(temp_1 = 0; temp_1 < iterations; temp_1++)
	{		
		altera_avalon_mutex_lock( mutex3, 1 );
		grayscale(*pros_3_start, *pros_3_dest, *pros_3_size);
		altera_avalon_mutex_unlock( mutex3);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex3, 1 );
		altera_avalon_mutex_unlock( mutex0);
		resize(*pros_3_start, *pros_3_dest, *pros_3_size, b_value);
		altera_avalon_mutex_unlock( mutex3);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex3, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		correctness(*pros_3_start, *pros_3_dest, *pros_3_size, actual_b_value);
		altera_avalon_mutex_unlock( mutex3);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex3, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		sobelFilter(*pros_3_start, *pros_3_dest, *pros_3_size);
		altera_avalon_mutex_unlock( mutex3);

		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_unlock( mutex0);
	}

	while (TRUE);

	return 0;
}
