#include <stdio.h>
#include "system.h"
#include "imageProcessing.h"

#define TRUE 1

int main()
{
	unsigned char* b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4008);
	unsigned char* actual_b_value = (unsigned char*)(SHARED_ONCHIP_BASE+4000);

	pros_4_status = (unsigned char*)(SHARED_ONCHIP_BASE+7203);
	*pros_4_status = 1;

	/* get the mutex device handle */
	mutex0 = altera_avalon_mutex_open( "/dev/mutex_0" );
	mutex4 = altera_avalon_mutex_open( "/dev/mutex_4" );

	/*memory location for image on shared on-chip memory */
	pros_4_start = (unsigned char**)(SHARED_ONCHIP_BASE+7090);
	pros_4_dest = (unsigned char**)(SHARED_ONCHIP_BASE+7100);
	pros_4_size = (unsigned char*)(SHARED_ONCHIP_BASE+7110);
	pros_4_iteration = (unsigned char*)(SHARED_ONCHIP_BASE+7114);

	/* local variables */
	unsigned char iterations = *pros_4_iteration;
	unsigned char temp_1;

	while(altera_avalon_mutex_first_lock(mutex4) == 1);

	for(temp_1 = 0; temp_1 < iterations; temp_1++)
	{		
		altera_avalon_mutex_lock( mutex4, 1 );
		grayscale(*pros_4_start, *pros_4_dest, *pros_4_size);
		altera_avalon_mutex_unlock( mutex4);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex4, 1 );
		altera_avalon_mutex_unlock( mutex0);
		resize(*pros_4_start, *pros_4_dest, *pros_4_size, b_value);
		altera_avalon_mutex_unlock( mutex4);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex4, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		correctness(*pros_4_start, *pros_4_dest, *pros_4_size, actual_b_value);
		altera_avalon_mutex_unlock( mutex4);
	
		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_lock( mutex4, 1 );
		altera_avalon_mutex_unlock( mutex0);	
		sobelFilter(*pros_4_start, *pros_4_dest, *pros_4_size);
		altera_avalon_mutex_unlock( mutex4);

		altera_avalon_mutex_lock( mutex0, 1);
		altera_avalon_mutex_unlock( mutex0);	
	}

	while (TRUE);

	return 0;
}
