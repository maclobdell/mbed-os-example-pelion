#include "mbed.h"
#include "blinker_app.h"
#include "eink_display_app.h"

#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_cy8ckit_028_epd.h"
#include "LCDConf.h"

#include "ThisThread.h"

Thread eink_thread;

#define DEMO_PAUSE_MS  6000

/* External global references */
extern GUI_CONST_STORAGE GUI_BITMAP bmCypressLogo_1bpp;

/* Display frame buffer cache */
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

/* Function prototypes */
void ShowPelionState(pelion_state_t);
void eink_display_thread(void);

/*******************************************************************************
* Function Name: void UpdateDisplay(void)
********************************************************************************
*
* Summary: This function updates the display with the data in the display
*            buffer.  The function first transfers the content of the EmWin
*            display buffer to the primary EInk display buffer.  Then it calls
*            the Cy_EINK_ShowFrame function to update the display, and then
*            it copies the EmWin display buffer to the Eink display cache buffer
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  It takes about a second to refresh the display.  This is a blocking function
*  and only returns after the display refresh
*
*******************************************************************************/
void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;

    /* Get the pointer to Emwin's display buffer */
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();

    /* Update the EInk display */
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);

    /* Copy the EmWin display buffer to the imageBuffer cache*/
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);

}

/*******************************************************************************
* Function Name: void ShowStartupScreen(void)
********************************************************************************
*
* Summary: This function displays the startup screen with Cypress Logo and
*            the demo description text
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ShowStartupScreen(void)
{
    /* Set foreground and background color and font size */
    GUI_SetFont(GUI_FONT_24_1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    //GUI_DrawBitmap(&bmCypressLogo_1bpp, 2, 2);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("CYPRESS", 132, 85);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("ARM PELION", 132, 115);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("FOTA DEMO", 132, 145);

    /* Send the display buffer data to display*/
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

/*******************************************************************************
* Function Name: void ShowPelionStatevoid)
********************************************************************************
*
* Summary: This function shows Pelion status
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

void ShowPelionState(pelion_state_t state)
{
    char state_str[12];
	  
	  switch(state){
			case CONNECTING:
				snprintf(state_str, 12, "%s", "CONNECTING  "); 			
				break;			
			case CONNECTED:
				snprintf(state_str, 12, "%s", "CONNECTED   ");
				break;			
			case REGISTERED:
				snprintf(state_str, 12, "%s", "REGISTERED  ");
				break;					
			case REQUESTED:
				snprintf(state_str, 12, "%s", "REQUESTED   ");
				break;					
			case PROCESSING:
				snprintf(state_str, 12, "%s", "PROCESSING  ");
				break;					
			case DOWNLOADING:
				snprintf(state_str, 12, "%s", "DOWNLOADING ");
				break;					
			case INSTALLING:
				snprintf(state_str, 12, "%s", "INSTALLING  ");
				break;					
			case COMPLETE:
				snprintf(state_str, 12, "%s", "COMPLETE    ");
				break;					
			default:
				snprintf(state_str, 12, "%s", "AMUSED      ");
    }
	
    /* Set font size, background color and text mode */
    GUI_SetFont(GUI_FONT_24_1);
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_NORMAL);

    /* Clear the display */
    GUI_Clear();

    /* Display page title */
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Connected to", 132, 5);
  	GUI_SetTextAlign(GUI_TA_HCENTER);
	  GUI_DispStringAt("Arm Pelion", 132, 20);

    /* Font24_1*/
    GUI_SetFont(GUI_FONT_24_1);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Firmware Update Status", 132, 70);

    /* Font24_1*/
    GUI_SetFont(GUI_FONT_24_1);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Firmware Version 1.2", 132, 144);

    /* Font32_1*/
    GUI_SetFont(GUI_FONT_32_1);
    GUI_SetTextAlign(GUI_TA_HCENTER);	
    GUI_DispStringAt(state_str, 132, 97);
	
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
//	UpdateDisplay(CY_EINK_FULL_4STAGE, false);
}

/*******************************************************************************
* Function Name: void ClearScreen(void)
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

/*****************************************************************************
* Function Name: eink_display_app_start
******************************************************************************
* Summary:
*   This functions initializes the display
*
*****************************************************************************/

int eink_display_app_start()
{   
    /* Initialize EmWin driver*/
    GUI_Init();
    
    /* Initialize EInk Controller */
    Cy_EINK_Start(15u, wait_ms);
    //Cy_EINK_Start(15u, ThisThread::sleep_for);
        
    /* Power on the display and check the status of the operation */
    if (Cy_EINK_Power(CY_EINK_ON) == CY_EINK_SUCCESS)
    {
        /* If powering of EINK display successful, continue with the demo */
		    
				eink_thread.start(eink_display_thread);
				return 0;	
		}else{
			  /*return error*/
			  return 1;
		}		
}
        
/*****************************************************************************
* Function Name: main()
******************************************************************************
* Summary:
*   Main function that starts a thread for the display
*
*****************************************************************************/

void eink_display_thread()
{
    /* Show the startup screen */
    ShowStartupScreen();
        
    /* Display startup screen for 3 seconds */
    //wait_ms(3000);
    ThisThread::sleep_for(3000);

		/* start the blinker thread */
		blinker_start();  

   while(1)
	 {
		 
			 /*Get state */

	    /*Display state */
			blinker_color_set(LED_COLOR_BLUE);
			blinker_mode_set(BLINKER_MODE_SOLID);
			
	    ShowPelionState(CONNECTING);
			blinker_color_set(LED_COLOR_GREEN);
			blinker_rate_set(2);
			blinker_mode_set(BLINKER_MODE_BLINKING);
	    //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
							
			ShowPelionState(CONNECTED);
			blinker_color_set(LED_COLOR_GREEN);
			blinker_mode_set(BLINKER_MODE_SOLID);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
			
			ShowPelionState(REGISTERED);
			blinker_color_set(LED_COLOR_BLUE);
			blinker_mode_set(BLINKER_MODE_SOLID);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
			
			ShowPelionState(REQUESTED);
			blinker_color_set(LED_COLOR_BLUE);		
			blinker_rate_set(2);
			blinker_mode_set(BLINKER_MODE_BLINKING);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
      			
			ShowPelionState(PROCESSING);
			blinker_color_set(LED_COLOR_BLUE);		
			blinker_rate_set(3);
			blinker_mode_set(BLINKER_MODE_BLINKING);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
			
			ShowPelionState(DOWNLOADING);
			blinker_color_set(LED_COLOR_RED);		
			blinker_rate_set(3);		
			blinker_mode_set(BLINKER_MODE_BLINKING);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
			
			ShowPelionState(INSTALLING);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
					
			ShowPelionState(COMPLETE);
			blinker_color_set(LED_COLOR_GREEN);
			blinker_mode_set(BLINKER_MODE_SOLID);
      //wait_ms(DEMO_PAUSE_MS);
      ThisThread::sleep_for(DEMO_PAUSE_MS);
			
   } 
}
