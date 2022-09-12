 /** 
 **************************************************************
 * @file mylib/s4593321_user_button.c
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief User Button
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"

int user_button_counter = 0;

/*
 * Initialise Hardware
 */
void s4593321_user_button_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	// Enable GPIO Clock
	__GPIOC_CLK_ENABLE();

    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13);	//Set fast speed.
	GPIOC->PUPDR &= ~(0x03 << (13 * 2));			//Clear bits for no push/pull
	GPIOC->MODER &= ~(0x03 << (13 * 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->RTSR |= EXTI_RTSR_TR13;	//enable rising dedge
	EXTI->FTSR &= ~EXTI_FTSR_TR13;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM13;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	BRD_LEDGreenToggle();
}

/*
 * Push Button callback
 */
void pb_callback(uint16_t GPIO_Pin) {

	// Check if the pushbutton pin was pressed.
	if (GPIO_Pin == 13) {
		user_button_counter = user_button_counter + 1;
		BRD_LEDGreenToggle(); 		//Toggle Blue LED

	}
}

/*
 * Interrupt handler (ISR) for EXTI 15 to 10 IRQ Handler
 * Note ISR should only execute a callback
 */ 
void EXTI15_10_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

	// PR: Pending register
	if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) {

		// cleared by writing a 1 to this bit
		EXTI->PR |= EXTI_PR_PR13;	//Clear interrupt flag.

		pb_callback(13);   // Callback for C13
	}
}

/*
 * Resets counter to 0
 */
void s4593321_user_button_reset(void){
    user_button_counter = 0;
}

int s4593321_user_button_get(void){
    return(user_button_counter);
}