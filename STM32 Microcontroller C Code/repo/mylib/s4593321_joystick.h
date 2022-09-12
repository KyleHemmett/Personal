 /** 
 **************************************************************
 * @file mylib/s4593321_joysick.h
 * @author Kyle Hemmett - s4593321
 * @date 03/03/2022
 * @brief joystick
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4593321_reg_joystick_pb_init() - intialise 
 * s4593321_reg_joystick_press_get() - get 
 *************************************************************** 
 */
#ifndef S4593321_JOYSTICK_H
#define S4593321_JOYSTICK_H


#define S4593321_REG_JOYSTICK_X_ZERO_CAL_OFFSET 0
#define S4593321_REG_JOYSTICK_Y_ZERO_CAL_OFFSET 0

#define S4593321_REG_JOYSTICK_X_READ() s4593321_joystick_readxy(ADC_Handlex)
#define S4593321_REG_JOYSTICK_Y_READ() s4593321_joystick_readxy(ADC_Handley)

ADC_HandleTypeDef ADC_Handlex;
ADC_ChannelConfTypeDef AdcChanConfigx;
ADC_HandleTypeDef ADC_Handley;
ADC_ChannelConfTypeDef AdcChanConfigy;


void s4593321_reg_joystick_pb_init(void);
void s4593321_reg_joystick_pb_isr(uint16_t GPIO_Pin);
int s4593321_reg_joystick_press_get(void);
int s4593321_joystick_readxy(ADC_HandleTypeDef ADC_Handler);
void s4593321_reg_joystick_press_reset(void);
void EXTI3_IRQHandler(void);



#endif