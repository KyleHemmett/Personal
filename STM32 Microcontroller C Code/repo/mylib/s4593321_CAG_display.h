 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_display.h
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief Display
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */
#ifndef S4593321_CAG_DISPLAY_H
#define S4593321_CAG_DISPLAY_H


#define RECEIVERTASK_PRIORITY     (tskIDLE_PRIORITY + 2)
#define RECEIVERTASK_STACK_SIZE   (configMINIMAL_STACK_SIZE * 2)

void display_grid(int pixels[64][16]);
void receiver_task(void);
void display_task(void);

#endif