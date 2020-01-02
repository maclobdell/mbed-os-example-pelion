#ifndef __EINK_DISPLAY_APP__
#define __EINK_DISPLAY_APP__

enum pelion_state_t {
	CONNECTING, 
	CONNECTED,
	REGISTERED,
	REQUESTED,
	PROCESSING,
	DOWNLOADING,
	INSTALLING,
	COMPLETE
}; 

/* Function prototypes */
int eink_display_app_start(void);
int set_pelion_state(pelion_state_t state);
pelion_state_t get_pelion_state(void);
int set_pelion_download_percent(uint8_t p);
uint8_t get_pelion_download_percent(void);

#endif
