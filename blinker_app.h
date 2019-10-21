#ifndef __BLINKER_APP__
#define __BLINKER_APP__


void blinker_init(void);

void blinker_rate_increase(void);

void blinker_rate_decrease(void);

void blinker_rate_set(uint32_t);

uint32_t blinker_rate_get(void);

#endif