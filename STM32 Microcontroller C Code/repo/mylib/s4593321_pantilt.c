 /** 
 **************************************************************
 * @file mylib/s4593321_joysick.c
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

#include "board.h"
#include "processor_hal.h"
#include "s4593321_pantilt.h"


/*
 * Initialise Hardware
 */
void s4593321_reg_pantilt_init(void) {

	// Enable GPIOC Clock
	__GPIOE_CLK_ENABLE();

    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (11 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (11 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (11 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << (3 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (3 * 4));	//Set Alternate Function for pin (lower ARF register) 

	GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (9 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (9 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (9 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << (1 * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << (1 * 4));	//Set Alternate Function for pin (lower ARF register) 

	// Timer 3 clock enable
	__TIM1_CLK_ENABLE();

	/* Compute the prescaler value
	   Set the clock prescaler to 50kHz
	   SystemCoreClock is the system clock frequency */
	TIM1->PSC = ((SystemCoreClock / 2) / TIMER_RUNNING_FREQ) - 1;

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM1->CR1 &= ~TIM_CR1_DIR; 

	TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 		//Set pulse period to ~1s 
	TIM1->CCR2 = PWM_PERCENT2TICKS_DUTYCYCLE(0);	//Set pulse width to 0% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 12); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC2PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC2E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter

	// Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
	TIM1->CR1 &= ~TIM_CR1_DIR; 

	TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 		//Set pulse period to ~1s 
	TIM1->CCR1 = PWM_PERCENT2TICKS_DUTYCYCLE(0);	//Set pulse width to 0% duty cycle.

	TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear OC2M (Channel 2) 
	TIM1->CCMR1 |= (0x6 << 4); 		// Enable PWM Mode 1, upcounting, on Channel 2 
	TIM1->CCMR1 |= (TIM_CCMR1_OC1PE); 	// Enable output preload bit for channel 2 
	
	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC1E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC2NE; 	// Clear CC2NE Bit for active high output
	
	/* Set Main Output Enable (MOE) bit
	   Set Off-State Selection for Run mode (OSSR) bit
	   Set Off-State Selection for Idle mode (OSSI) bit */
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter

}


void s4593321_pantilt_angle_write(int type, int angle) {


	if (type == 0){
		float dutycycle = (((float)1/18)*(float)(angle + 20)) + 7.25;
		PWM_DUTYCYCLE_CHANGE_PAN(PWM_PERCENT2TICKS_DUTYCYCLE(dutycycle));
	}
	else if (type == 1){
		float dutycycle = (((float)1/18)*(float)(angle)) + 7.25;
		PWM_DUTYCYCLE_CHANGE_TILT(PWM_PERCENT2TICKS_DUTYCYCLE(dutycycle));
	}
}

float s4593321_pantilt_angle_read(int type){

	float angle = 0;

	if (type == 0){
		angle = (TIM1->CCR1)/(float)100;
		angle = round(((angle - 7.25)/(1/(float)18)) - 20);
		
	}
	else if (type == 1){
		angle = (TIM1->CCR2)/(float)100;
		angle = round(((angle - 7.25)/(1/(float)18)));
	}
	return angle;
}

	