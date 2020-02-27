#include "hello_ucosii.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_performance_counter.h"
#include "imageProcessing.h"
#include "images/images_alt.h"

#define PERFORMANCE_COUNT 32

/*********************************************************/
/************** Task *************************************/
/*********************************************************/

void task1(void* pdata)
{
  	INT8U err;
  	while (1)
  	{
		OSSemPend(Task1TmrSem, 0, &err);

		if(iterations <= 0)
			break;

		#ifdef DEBUG 
    		printf("Hello from task 1 - Iteration No - %d, Image No - %d\n", iterations, sequence_no);
		#endif

		dest_image = grayscale(sequence1[sequence_no]);
		OSSemPost(Task2TmrSem);
  	}

	OSTaskDel(OS_PRIO_SELF);
}


void task2(void* pdata)
{
  	INT8U err;
  	while (1)
  	{
		OSSemPend(Task2TmrSem, 0, &err);

		if(iterations <= 0)
			break;

		#ifdef DEBUG 
    		printf("Hello from task 2 - Iteration No - %d, Image No - %d\n", iterations, sequence_no);
		#endif

		if(dest_image == NULL) {
			printf("Task - 2 :Image start address cannot be NULL\n");
			break;
		}
		
		resize(dest_image, b_value);
		OSSemPost(Task3TmrSem);
  	}

	OSTaskDel(OS_PRIO_SELF);
}

void task3(void* pdata)
{
	INT8U err;
	while (1)
  	{
		OSSemPend(Task3TmrSem, 0, &err);

		if(iterations <= 0)
			break;

		#ifdef DEBUG 
    		printf("Hello from task 3 - Iteration No - %d, Image No - %d\n", iterations, sequence_no);
		#endif

		if(dest_image == NULL) {
			printf("Task - 3 : Image start address cannot be NULL\n");
			break;
		}
		
		correctness(dest_image, b_value);
		OSSemPost(Task4TmrSem);
  	}

	OSTaskDel(OS_PRIO_SELF);
}


void task4(void* pdata)
{
  	INT8U err;
	while (1)
  	{
		OSSemPend(Task4TmrSem, 0, &err);

		if(iterations <= 0)
			break;

		#ifdef DEBUG 
    		printf("Hello from task 4 - Iteration No - %d, Image No - %d\n", iterations, sequence_no);
		#endif

		if(dest_image == NULL) {
			printf("Task - 4 : Image start address cannot be NULL\n");
			break;
		}
		
		sobelFilter(dest_image);
		OSSemPost(Task5TmrSem);
  	}

	OSTaskDel(OS_PRIO_SELF);
}

void task5(void* pdata)
{
	INT8U err;
	while (iterations > 0)
  	{
		OSSemPend(Task5TmrSem, 0, &err);

		#ifdef DEBUG 
    		printf("Hello from task 5 - Iteration No - %d, Image No - %d\n", iterations, sequence_no);
		#endif

		if(dest_image == NULL) {
			printf("Task - 5 : Image start address cannot be NULL\n");
			break;
		}
		
		toAsciiArt(dest_image);

		#ifdef DEBUG
			
		printf("\n\n ******Output Image****** \n");
		length = dest_image[0] * dest_image[1];
		lengthX = dest_image[0];
		dest_image = dest_image + 3;

		for(temp_1 = 0; temp_1 < length; temp_1++)
		{
			if((temp_1 % lengthX) == 0)
				printf("\n");
			printf("%c",dest_image[temp_1]);
		}
		printf("\n\n");
		dest_image = dest_image - 3;
		#endif

		free(dest_image);
		dest_image = NULL;

		sequence_no++;
		if((sequence_no & (sequence1_length - 1)) == 0) {
			iterations--;
			sequence_no = 0;
		}

		OSSemPost(Task1TmrSem);
  	}

	#ifdef PERFORMANCE	
		PERF_END(PERFORMANCE_COUNTER_0_BASE,1);
		PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);

		total_execution_time = ((unsigned int)perf_get_section_time((void*)PERFORMANCE_COUNTER_0_BASE, 1) - timer_overhead_perf)
					/((unsigned int)alt_get_cpu_freq()/1000);

		execution_time_per_image = total_execution_time / (PERFORMANCE_COUNT*sequence1_length);

		throughput = 1000/ execution_time_per_image;
	
		printf("\nPerformance Counter Frequency : %d\n", (unsigned int)alt_get_cpu_freq());
		printf("Performance Counter in ticks : %d\n", 
			(unsigned int) (perf_get_section_time((void*)PERFORMANCE_COUNTER_0_BASE, 1) - timer_overhead_perf));
		printf("Number of Images Executed : %d\n", (PERFORMANCE_COUNT*sequence1_length));
		printf("Total Execution Time : %d ms\n", total_execution_time);
		printf("Execution Time Per Image : %d ms\n", execution_time_per_image);
		printf("Throughput : %d (images/second)\n", throughput);
	
	#endif

	while(1);
}

void StartTask(void* pdata)
{
	INT8U err;

	#ifdef DEBUG
	iterations = 1;
	#endif
	#ifdef PERFORMANCE
	iterations = PERFORMANCE_COUNT;
	#endif
  

	/* Creation of Kernel Objects */

	Task1TmrSem = OSSemCreate(0);   
	Task2TmrSem = OSSemCreate(0);   
	Task3TmrSem = OSSemCreate(0);   
	Task4TmrSem = OSSemCreate(0);   
	Task5TmrSem = OSSemCreate(0);     

	/* Create statistics task */

	OSStatInit();

	/*****************************************************************/
	/************** Creating Tasks in the system *********************/ 
	/*****************************************************************/

	/******* TASK 1 ********/
	err=OSTaskCreate(task1,
      		          NULL,
       		          (void *)&task1_stk[TASK_STACKSIZE-1],
       		          TASK1_PRIORITY
       		          );

	#ifdef DEBUG
	if (err == OS_ERR_NONE) { //if start successful
  		printf("Task 1 created\n");
    	}
	#endif 

	/******* TASK 2 ********/
	err=OSTaskCreate(task2,
        	  	  NULL,
        	  	  (void *)&task2_stk[TASK_STACKSIZE-1],
        	  	  TASK2_PRIORITY
        	  	  );

	#ifdef DEBUG
     	if (err == OS_ERR_NONE) { //if start successful
      		printf("Task 2 created\n");
    	}
	#endif

	/******* TASK 3 ********/
	err=OSTaskCreate(task3,
        		  NULL,
        		  (void *)&task3_stk[TASK_STACKSIZE-1],
        		  TASK3_PRIORITY
        		  );

	#ifdef DEBUG
    	if (err == OS_ERR_NONE) { //if start successful
      		printf("Task 3 created\n");
    	}
	#endif  

	/******* TASK 4 ********/
	err=OSTaskCreate(task4,
        		  NULL,
        		  (void *)&task4_stk[TASK_STACKSIZE-1],
        		  TASK4_PRIORITY
        		  );

	#ifdef DEBUG
    	if (err == OS_ERR_NONE) { //if start successful
      		printf("Task 4 created\n");
    	}
	#endif

	/******* TASK 5 ********/
	err=OSTaskCreate(task5,
        		  NULL,
        		  (void *)&task5_stk[TASK_STACKSIZE-1],
        		  TASK5_PRIORITY
        		  );

	#ifdef DEBUG
     	if (err == OS_ERR_NONE) { //if start successful
      		printf("Task 5 created\n");
    	}
	
	printf("All Tasks and Kernel Objects generated!\n");
	#endif

	/********************************************************************************/
	/********************** PERFORMANCE COUNTER INITIALIZATION **********************/
	/********************************************************************************/
	
	#ifdef PERFORMANCE
	
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

	timer_overhead_perf = 0;
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
 
	OSSemPost(Task1TmrSem);

	/* Task deletes itself */

	OSTaskDel(OS_PRIO_SELF);
}


int main(void) {


  	printf("MicroC/OS-II-Vesion: %1.2f\n", (double) OSVersion()/100.0);
     
  	OSTaskCreateExt(
		 StartTask, // Pointer to task code
        	 NULL,      // Pointer to argument that is
        	            // passed to task
        	 (void *)&StartTask_Stack[TASK_STACKSIZE-1], // Pointer to top
						     // of task stack 
        	 STARTTASK_PRIO,
        	 STARTTASK_PRIO,
        	 (void *)&StartTask_Stack[0],
        	 TASK_STACKSIZE,
        	 (void *) 0,  
        	 OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
         
  	OSStart();
  
  	return 0;
}
