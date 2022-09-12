 /** 
 **************************************************************
 * @file pf/main.c
 * @author Kyle Hemmett - s4593321
 * @date 11/04/2022
 * @brief main
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "debug_log.h"
#include "s4593321_CAG_simulator.h"
#include "s4593321_CAG_display.h"
#include "s4593321_CAG_grid.h"
#include "s4593321_user_button.h"
#include "s4593321_CAG_Mnemonic.h"


/*
 * Starts all the other tasks, then starts the scheduler.
 */

int main( void ) {
	// INITALISE ALL TASKS
	BRD_debuguart_init();
	simulator_task();
	display_task();
	grid_task();
	Mnemonic_task();
	vTaskStartScheduler();
	return 0;
}

