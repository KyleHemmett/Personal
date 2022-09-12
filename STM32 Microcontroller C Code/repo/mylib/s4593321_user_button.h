 /** 
 **************************************************************
 * @file mylib/s4593321_user_button
 * @date 31/05/2022
 * @brief User Button
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */
#ifndef S4593321_USER_BUTTON_H
#define S4593321_USER_BUTTON_H

void s4593321_user_button_init(void);
void pb_callback(uint16_t GPIO_Pin);
void s4593321_user_button_reset(void);
int s4593321_user_button_get(void);

#endif