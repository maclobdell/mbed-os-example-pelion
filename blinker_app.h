#ifndef __BLINKER_APP__
#define __BLINKER_APP__


void blinker_start(void);
void blinker_rate_increase(void);
void blinker_rate_decrease(void);
void blinker_rate_set(uint32_t);
uint32_t blinker_rate_get(void);
void blinker_color_set(uint8_t);
void blinker_mode_set(uint8_t);
   
#define LED_COLOR_RED 1
#define LED_COLOR_GREEN 2
#define LED_COLOR_BLUE 3
#define BLINKER_MODE_OFF 0
#define BLINKER_MODE_SOLID 1
#define BLINKER_MODE_BLINKING 2

#endif
