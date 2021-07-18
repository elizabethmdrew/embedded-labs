#include <stdio.h>
#include <cmsis_os2.h>
#include <rtx_os.h>
#include <stdio.h> 
#include <stdbool.h>
#include <lpc17xx.h>
#include "random.h"

/* #define BUTTON_FLAG 0x00000001UL //A
osEventFlagsId_t eventFlags; */

//A
osMessageQueueId_t queue; 
int msg; 

void setupPushButton(void)
{
	LPC_GPIOINT->IO2IntEnF|=(1<<10);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void ENINT_IRQHandler(void)
{
	LPC_GPIOINT -> IO2IntClr|= (1<<10); 
	//B
	osMessageQueuePut(queue,&msg,NULL,NULL);
}

void t1(void *arg)
{
	while(true)
	{
		//C
		osMessageQueueGet(queue, &msg, NULL, osWaitForever); 
		printf("button pushed\n");
	}
}
int main (void)
{
	osKernelInitialize(); 
	//D
	queue = osMessageQueueNew(10, sizeof(int), NULL); 
	setupPushButton(); 
	osThreadNew(t1, NULL, NULL); 
	osKernelStart(); 
	for(;;);
}


 /*
//define number of servers and thus number of queues
#define N_SERVERS 2 

//GLOBALS
//create an array of message queues equal to number of servers
osMessageQueueId_t queue_ID[N_SERVERS]; 

//initialize message variable
int msg; 

//initialize recording variables
uint32_t server_id[N_SERVERS]; 
uint32_t msgs_sent[N_SERVERS]; 
uint32_t msgs_received[N_SERVERS];  
uint32_t num_overflows[N_SERVERS]; 

int elapsed_time = -1; 
int seconds = 0;

void server(void *id_num)
{
	//Cast and dereference to get queue identifier
	uint32_t serv_ID = *(uint32_t*)id_num;
	
	while(true)
	{
		//Determine nticks for delay and delay
		uint32_t nticks = ((next_event()*osKernelGetTickFreq())/10)>>16;
		osDelay(nticks); 
		
		//Get message from message queue
		osMessageQueueGet(queue_ID[serv_ID], &msg, NULL, osWaitForever); 
		//Increment messages received
		msgs_received[serv_ID] += 1; 
	}
}

void client(void *arg)
{
	while(true)
	{
		//Determine nticks for delay and delay
		uint32_t nticks = ((next_event()*osKernelGetTickFreq())/9)>>16;
		osDelay(nticks); 
		
		//Use for loop to alternate between queues
		for (int i = 0; i < N_SERVERS; i++)
		{
			//Put message in message queue
			if (osMessageQueuePut(queue_ID[i], &msg, 0, 0) != osErrorResource)
				msgs_sent[i] += 1;	
			//Queue is full
			else 
				num_overflows[i] += 1; 
		}	
	}
}
	
void monitor_print(void*arg)
{
    while(true)
    {
        if(seconds>elapsed_time)
        {
						printf("Elapsed Time: %ds, queue1Sent: %d, queue1Received: %d, queue1Overflows: %d, queue2Sent: %d, queue2Received: %d, queue2Overflows: %d\n",
						seconds, msgs_sent[0], msgs_received[0], num_overflows[0],msgs_sent[1], msgs_received[1], num_overflows[1]);
                          
             //delay by 1s (ticks/sec * 1 sec) to run the thread once/s
            elapsed_time = seconds; 
        }
    }
}

//Thread for seconds count, speeds up print
void monitor_time(void *arg)
{
    while(true)
    {
        osDelay(osKernelGetTickFreq()); //(ticks/sec)*1sec
        ++seconds; 
    }        
}

void init()
{
		for(int i = 0; i<N_SERVERS; i++)
		{
			//Create server threads and queues
			queue_ID[i] = osMessageQueueNew(10, sizeof(int), NULL); 
			server_id[i] = i; 
			osThreadNew(server, &server_id[i], NULL); 
		}
}
	
	
int main (void)
{	
	//Initialize Kernel
	osKernelInitialize(); 	
	//Initialize server threads and queues using initialization function
  init(); 
	//Initialize monitor threads
  osThreadNew(monitor_print, NULL, NULL);
	osThreadNew(monitor_time, NULL, NULL);
	//Initialize client thread 
	osThreadNew(client, NULL, NULL);
	//Start the Kernel
	osKernelStart();
}
*/
