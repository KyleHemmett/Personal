 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_Mnemonic.h
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief Mnemonic
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */
#ifndef S4593321_CAG_MNEMONIC_H
#define S4593321_CAG_MNEMONIC_H

#include "FreeRTOS_CLI.h"


// Task Priorities (Idle Priority is the lowest priority)
#define CLI_PRIORITY					( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define CLI_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )

void cliTask(void);
void Mnemonic_task(void);


#endif