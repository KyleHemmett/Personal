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
#include "s4593321_joystick.h"

static int joystick_press_counter;
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;


/*
 * Initialise Hardware
 */
void s4593321_reg_joystick_pb_init(void) {

	// Enable GPIOC Clock
	__GPIOA_CLK_ENABLE();

    GPIOA->OSPEEDR |= (GPIO_SPEED_FAST << 3);	//Set fast speed.
	GPIOA->PUPDR &= ~(0x03 << (3 * 2));			//Clear bits for no push/pull
	GPIOA->MODER &= ~(0x03 << (3* 2));			//Clear bits for input mode

	// Enable EXTI clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//select trigger source (port c, pin 13) on EXTICR4.
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR1_EXTI3_PC;
	SYSCFG->EXTICR[3] &= ~(0x000F);

	EXTI->RTSR &= ~EXTI_RTSR_TR3;	//able rising edge
	EXTI->FTSR |= EXTI_FTSR_TR3;	//disable falling edge
	EXTI->IMR |= EXTI_IMR_IM3;		//Enable external interrupt

	//Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
	HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);


	__GPIOC_CLK_ENABLE();

	// Initalise PC0 and PC3 as an Analog input.
  	GPIOC->MODER |= (0x03 << (0 * 2));			//Set bits for Analog input mode
	GPIOC->MODER |= (0x03 << (3 * 2));			//Set bits for Analog input mode  

  	GPIOC->OSPEEDR &= ~(0x03<<(0 * 2));
	GPIOC->OSPEEDR |= 0x02<<(0 * 2);  			// Fast speed

	GPIOC->OSPEEDR &= ~(0x03<<(3 * 2));
	GPIOC->OSPEEDR |= 0x02<<(3 * 2);  			// Fast speed

	GPIOC->PUPDR &= ~(0x03 << (0 * 2));			//Clear bits for no push/pull
	GPIOC->PUPDR &= ~(0x03 << (3 * 2));			//Clear bits for no push/pull

	__ADC1_CLK_ENABLE();
						//Enable ADC1 clock

	ADC_Handlex.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	ADC_Handlex.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	ADC_Handlex.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	ADC_Handlex.Init.ScanConvMode          = DISABLE;
	ADC_Handlex.Init.ContinuousConvMode    = DISABLE;
	ADC_Handlex.Init.DiscontinuousConvMode = DISABLE;
	ADC_Handlex.Init.NbrOfDiscConversion   = 0;
	ADC_Handlex.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	ADC_Handlex.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	ADC_Handlex.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	ADC_Handlex.Init.NbrOfConversion       = 1;
	ADC_Handlex.Init.DMAContinuousRequests = DISABLE;
	ADC_Handlex.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&ADC_Handlex);		//Initialise ADC

	// Configure ADC Channel
	AdcChanConfigx.Channel = ADC_CHANNEL_13;					//PA3 has Analog Channel 3 connected
	AdcChanConfigx.Rank         = 1;
	AdcChanConfigx.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigx.Offset       = 0;

	HAL_ADC_ConfigChannel(&ADC_Handlex, &AdcChanConfigx);		//Initialise ADC Channel


	__ADC2_CLK_ENABLE();
						//Enable ADC1 clock

	ADC_Handley.Instance = (ADC_TypeDef *)(ADC2_BASE);						//Use ADC1 (For PA3)
	ADC_Handley.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	ADC_Handley.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	ADC_Handley.Init.ScanConvMode          = DISABLE;
	ADC_Handley.Init.ContinuousConvMode    = DISABLE;
	ADC_Handley.Init.DiscontinuousConvMode = DISABLE;
	ADC_Handley.Init.NbrOfDiscConversion   = 0;
	ADC_Handley.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	ADC_Handley.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	ADC_Handley.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	ADC_Handley.Init.NbrOfConversion       = 1;
	ADC_Handley.Init.DMAContinuousRequests = DISABLE;
	ADC_Handley.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&ADC_Handley);		//Initialise ADC

	// Configure ADC Channel
	AdcChanConfigy.Channel = ADC_CHANNEL_10;					//PA3 has Analog Channel 3 connected
	AdcChanConfigy.Rank         = 1;
	AdcChanConfigy.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigy.Offset       = 0;

	HAL_ADC_ConfigChannel(&ADC_Handley, &AdcChanConfigy);		//Initialise ADC Channel


}

int s4593321_joystick_readxy(ADC_HandleTypeDef ADC_Handler){

		unsigned int adc_value;

		HAL_ADC_Start(&ADC_Handler); // Start ADC conversion

		// Wait for ADC conversion to finish
		while (HAL_ADC_PollForConversion(&ADC_Handler, 10) != HAL_OK);

		if(ADC_Handler.Instance == (ADC_TypeDef *)(ADC2_BASE)){
			adc_value = ADC2->DR + S4593321_REG_JOYSTICK_X_ZERO_CAL_OFFSET;
		}
		else if(ADC_Handler.Instance == (ADC_TypeDef *)(ADC1_BASE)){
			adc_value = ADC1->DR + S4593321_REG_JOYSTICK_Y_ZERO_CAL_OFFSET;
		}

		return(adc_value);
		//Read ADC1 Data Register for converted value.
}

/*
 * Resets counter to 0
 */
void s4593321_reg_joystick_press_reset(void){
    joystick_press_counter = 0;
}

/*
 * Push Button callback
 */
void s4593321_reg_joystick_pb_isr(uint16_t GPIO_Pin) {

	// Check if the pushbutton pin was pressed.
	if (GPIO_Pin == 3 && ((HAL_GetTick() - previousMillis) > 150)) {
		
		if (joystick_press_counter < 1023){
			joystick_press_counter++; 
		}
		else {
			s4593321_reg_joystick_press_reset();
		}
        previousMillis = HAL_GetTick();

	}

	return;
}

/*
 * Returns the current joystick press counter
 */
int s4593321_reg_joystick_press_get(void){
    return(joystick_press_counter);
}


/*
 * Interrupt handler (ISR) for EXTI 15 to 10 IRQ Handler
 * Note ISR should only execute a callback
 */ 
void EXTI3_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI3_IRQn);

	// PR: Pending register
	if ((EXTI->PR & EXTI_PR_PR3) == EXTI_PR_PR3) {

		// cleared by writing a 1 to this bit
		EXTI->PR |= EXTI_PR_PR3;	//Clear interrupt flag.

		s4593321_reg_joystick_pb_isr(3);   // Callback for C13
	}

	
}