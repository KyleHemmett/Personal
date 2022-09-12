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
#ifndef S4593321_PANTILT_H
#define S4593321_PANTILT_H

#define TIMER_RUNNING_FREQ  				242500        			//Frequency (in Hz)
#define PWM_PULSE_WIDTH_SECONDS				2					//2s
#define PWM_PULSE_WIDTH_TICKS				10000		//Period in timer ticks
#define PWM_PERCENT2TICKS_DUTYCYCLE(value)	(uint16_t)(value*PWM_PULSE_WIDTH_TICKS/100)				//Duty Cycle on time in timer ticks
#define PWM_DUTYCYCLE_CHANGE_PAN(value) 		TIM1->CCR1=(uint16_t)value		
#define PWM_DUTYCYCLE_CHANGE_TILT(value) 		TIM1->CCR2=(uint16_t)value	

#define S4593321_REG_PANTILT_PAN_WRITE(angle) s4593321_pantilt_angle_write(0, angle)
#define S4593321_REG_PANTILT_TILT_WRITE(angle) s4593321_pantilt_angle_write(1, angle)
#define S4593321_REG_PANTILT_PAN_READ() s4593321_pantilt_angle_read(0)
#define S4593321_REG_PANTILT_TILT_READ() s4593321_pantilt_angle_read(1)


void s4593321_reg_pantilt_init(void);
void s4593321_pantilt_angle_write(int type, int angle);
float s4593321_pantilt_angle_read(int type);


#endif