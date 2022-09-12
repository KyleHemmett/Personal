 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_grid.c
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief Grid
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
#include "event_groups.h"
#include "s4593321_CAG_grid.h"
#include "s4593321_CAG_simulator.h"
#include "debug_log.h"
#include "s4593321_lta1000g.h"
#include "s4593321_user_button.h"

EventBits_t uxBits;
QueueHandle_t life_queue;
struct CaMessage life_message;

 int xPos = 0;
 int yPos = 0;
 int paused = 0;


/*
 * Give Task.
 */
void giveTask( void ) {

    unsigned char input_one;

	GPIO_InitTypeDef GPIO_InitStructure;

	lifectrlEventGroup = xEventGroupCreate();

	for (;;) {
        int button_state;
        button_state = s4593321_user_button_get();
        if(button_state % 2 == 0) {
            if ((input_one = BRD_debuguart_getc()) != '\0'){
                if (input_one == 'W'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_MOVE_UP);
                }
                if (input_one == 'A'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_MOVE_LEFT);
                }
                if (input_one == 'S'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_MOVE_DOWN);
                }
                if (input_one == 'D'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_MOVE_RIGHT);
                }
                if (input_one == 'X'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_SELECT_CELL);
                }
                if (input_one == 'Z'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_UNSELECT_CELL);
                }
                if (input_one == 'O'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_MOVE_ORIGIN);
                }
                if (input_one == 'P'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_TOGGLE_START);
                }
                if (input_one == 'C'){
                    uxBits = xEventGroupSetBits(lifectrlEventGroup, EVT_CLEAR_DISPLAY);
                }
            }
        }
        vTaskDelay(10);
	}
}

/*
 * Take Task.
 */
void takeTask( void ) {

    s4593321_reg_lta1000g_init();

	for (;;) {
    	uxBits = xEventGroupWaitBits(lifectrlEventGroup, LIFECTRL_EVENT, pdTRUE, pdFALSE, 10);
		if ((uxBits & EVT_MOVE_UP) != 0) {
            if(checkBounds(xPos, yPos - 1) == 1) {
                yPos = yPos - 1;
                debug_log("(%d,%d)\n\r", xPos, yPos);
                s4593321_reg_lta1000g_writeY(yPos);
                s4593321_reg_lta1000g_writeX(xPos);
            }
		}
        else if ((uxBits & EVT_MOVE_LEFT) != 0) {
            if(checkBounds(xPos - 1, yPos) == 1) {
                xPos = xPos - 1;
                debug_log("(%d,%d)\n\r", xPos, yPos);
                s4593321_reg_lta1000g_writeY(yPos);
                s4593321_reg_lta1000g_writeX(xPos);
            }
		}
        else if ((uxBits & EVT_MOVE_RIGHT) != 0) {
            if(checkBounds(xPos + 1, yPos) == 1) {
                xPos = xPos + 1;
                debug_log("(%d,%d)\n\r", xPos, yPos);
                s4593321_reg_lta1000g_writeY(yPos);
                s4593321_reg_lta1000g_writeX(xPos);
            }
		}
        else if ((uxBits & EVT_MOVE_DOWN) != 0) {
            if(checkBounds(xPos, yPos + 1) == 1) {
                yPos = yPos + 1;
                debug_log("(%d,%d)\n\r", xPos, yPos);
                s4593321_reg_lta1000g_writeY(yPos);
                s4593321_reg_lta1000g_writeX(xPos);
            }
		}
        else if ((uxBits & EVT_SELECT_CELL) != 0) {
            debug_log("LIFE CREATED\n\r");
            life_message.cell_x = xPos;
            life_message.cell_y = yPos;
            life_message.type = 00010001;
            xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
		}
        else if ((uxBits & EVT_UNSELECT_CELL) != 0) {
            debug_log("LIFE REMOVED\n\r");
            life_message.cell_x = xPos;
            life_message.cell_y = yPos;
            life_message.type = 00010000;
            xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
		}
        else if ((uxBits & EVT_MOVE_ORIGIN) != 0) {
            yPos = 0;
            xPos = 0;
            debug_log("(%d,%d)\n\r", xPos, yPos);
            s4593321_reg_lta1000g_writeY(yPos);
            s4593321_reg_lta1000g_writeX(xPos);

		}
        else if ((uxBits & EVT_TOGGLE_START) != 0) {
            debug_log("START/STOP\n\r");
            if (paused == 0) {
                life_message.type = 11111111;
                paused = 1;
            }
            else {
                life_message.type = 10000000;
                paused = 0;
            }
            xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);   
		}
        else if ((uxBits & EVT_CLEAR_DISPLAY) != 0) {
            debug_log("CLEAR\n\r");
            life_message.type = 11000000;
            xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);

		}

		vTaskDelay(1);
	}
}

// Checks if the given coordinate are in bounds if not returns 0
int checkBounds(int xPos, int yPos){
    if (xPos < 0 || xPos >= WIDTH || yPos < 0 || yPos >= HEIGHT){
        return 0;
    }
    else{
        return 1;
    }
}

// Grid task called in the main
void grid_task(void) {
    struct CaMessage new;
    life_queue = xQueueCreate(10, sizeof(new.type)+sizeof(new.cell_x)+sizeof(new.cell_y));
    xTaskCreate( (void *) &giveTask, (const signed char *) "GIVE", GIVETASK_STACK_SIZE, NULL, GIVETASK_PRIORITY, NULL );
    xTaskCreate( (void *) &takeTask, (const signed char *) "TAKE", TAKETASK_STACK_SIZE, NULL, TAKETASK_PRIORITY, NULL );
}

