/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "ThisThread.h"

#define MAX_INDEX 7

Thread blinker;
//Semaphore blinker_sem;
DigitalOut blinker_led(LED1);

uint32_t blink_rate[8] = {0,25,50,100,250,500,750,1000};

int32_t blink_index = 0;


void blinker_thread(void)
{
	while(true){
		if(blink_rate[blink_index] !=0)
		{	
			blinker_led = !blinker_led;
			ThisThread::sleep_for(blink_rate[blink_index]);
		}else{
		    ThisThread::sleep_for(200);
		}		
	}
}

void blinker_init(void)
{
	
    blinker.start(blinker_thread);
	
   /*
   while (true) {
        printf("%d\r\n",blink_rate[blink_index]);
		ThisThread::sleep_for(1000);
		
    }
    */
}

void blinker_rate_increase(void)
{
	//blinker_sem.acquire();  

	blink_index++;
		
	if (blink_index > MAX_INDEX)
	{
		blink_index = MAX_INDEX;
	}
	//blinker_sem.release();  
	
}

void blinker_rate_decrease(void)
{
	//blinker_sem.acquire();  
	
	blink_index--;

	if (blink_index < 0)
	{
		blink_index = 0;
	}
	//blinker_sem.release();  	
}

void blinker_rate_set(uint32_t index)
{
	//blinker_sem.acquire();  
	
	blink_index = (int32_t) index;
	
    if (blink_index > MAX_INDEX)
	{
		blink_index = MAX_INDEX;
	}
	if (blink_index < 0)
	{
		blink_index = 0;
	}
	//blinker_sem.release();  
}

uint32_t blinker_rate_get(void)
{
	//blinker_sem.acquire(); 
	
	uint32_t ret = (uint32_t) blink_index;	
	
	//blinker_sem.release();  
	
    return ret;	
	
}