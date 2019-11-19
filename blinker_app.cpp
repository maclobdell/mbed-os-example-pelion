/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "ThisThread.h"

#define MAX_INDEX 7

Thread blinker;
DigitalOut blinker_led(LED1);

uint32_t blink_rate[8] = {1000,750,500,250,100,50,25,5};

volatile int32_t blink_index = 0;


void blinker_thread(void)
{
    while(true){
        if(blink_rate[blink_index] !=0)
        {
            blinker_led = !blinker_led;
            ThisThread::sleep_for(blink_rate[blink_index]);
        } else {
            ThisThread::sleep_for(200);
        }
    }
}

void blinker_init(void)
{

    blinker.start(blinker_thread);
}

void blinker_rate_increase(void)
{
    blink_index++;

    if (blink_index > MAX_INDEX)
    {
        blink_index = MAX_INDEX;
    }
}

void blinker_rate_decrease(void)
{
    blink_index--;

    if (blink_index < 0)
    {
        blink_index = 0;
    }
}

void blinker_rate_set(uint32_t index)
{
    blink_index = (int32_t) index;

    if (blink_index > MAX_INDEX)
    {
        blink_index = MAX_INDEX;
    }
    if (blink_index < 0)
    {
        blink_index = 0;
    }
}

uint32_t blinker_rate_get(void)
{
    uint32_t ret = (uint32_t) blink_index;
    return ret;
}
