#include <stdio.h>
#include "includes.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "sys/alt_alarm.h"
#include "system.h"

#define HW_TIMER_PERIOD 100 /* 100ms */

/* Definition of Task Stacks */

#define   TASK_STACKSIZE       2048

OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];
OS_STK    task4_stk[TASK_STACKSIZE];
OS_STK    task5_stk[TASK_STACKSIZE];
OS_STK    StartTask_Stack[TASK_STACKSIZE]; 

/* Definition of Task Priorities */

#define STARTTASK_PRIO      1
#define TASK1_PRIORITY      10
#define TASK2_PRIORITY      12
#define TASK3_PRIORITY      14
#define TASK4_PRIORITY      16
#define TASK5_PRIORITY      18

/* Definition of Task Periods (ms) */

#define TASK1_GRAYSCALE_PERIOD 400
#define TASK2_RESIZE_PERIOD 400
#define TASK3_CORRECTNESS_PERIOD 400
#define TASK4_SOBEL_PERIOD 400
#define TASK5_ASCII_PERIOD 400

/* Global variables */

int delay; // Delay of HW-timer
unsigned char* dest_image = NULL;
unsigned char b_value[2];
unsigned char iterations;
unsigned char sequence_no;

short temp_1;
#ifdef DEBUG
short length, lengthX;
#endif

#ifdef PERFORMANCE
unsigned int timer_overhead_perf;
unsigned int total_execution_time;
unsigned int execution_time_per_image;
unsigned int throughput;
#endif

/* Semaphores */

OS_EVENT *Task1TmrSem = NULL;
OS_EVENT *Task2TmrSem = NULL;
OS_EVENT *Task3TmrSem = NULL;
OS_EVENT *Task4TmrSem = NULL;
OS_EVENT *Task5TmrSem = NULL;

/*SW-Timer */
OS_TMR *Task1Tmr = NULL;
OS_TMR *Task2Tmr = NULL;

