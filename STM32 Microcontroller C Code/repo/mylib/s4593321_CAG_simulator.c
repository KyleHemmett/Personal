 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_simulator.c
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief Simulator
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
#include "s4593321_CAG_simulator.h"
#include "s4593321_CAG_grid.h"
#include "s4593321_CAG_display.h"
#include "s4593321_user_button.h"
#include "debug_log.h"

QueueHandle_t display_queue;
struct Message display_message;
EventGroupHandle_t lifectrlEventGroup;
struct CaMessage lifeRecvMessage;


// if it is paused/stopped paused == 0 else paused == 1
int pasued = 0;

// sets up grid full of 0's
void setupGrid(void) {
    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            display_message.grid[x][y] = 0;
        }
    }
}

// sender task to send to the display
void senderTask(void){

    setupGrid();

    for(;;) {
        if (pasued != 0) {
            checkRules();
        }
        if (display_queue != NULL) {
            xQueueSendToFront(display_queue, (void *) &display_message, (portTickType) 2);
        }
        vTaskDelay(1000);
    }
}

// revicever task to revice if life or anything rldr has been created
void lifeReceiverTask(void) {

    s4593321_user_button_init();
    for(;;) {
        if (life_queue != NULL) {
            if (xQueueReceive(life_queue, &lifeRecvMessage, 10)) {
                setGrid();
            }
            else {
            }
        }
        vTaskDelay(1000);
    }
}

// simulator task for the main
void simulator_task(void) {
    struct Message new;
    display_queue = xQueueCreate(10, sizeof(new.grid));
    xTaskCreate( (void *) &senderTask, (const signed char *) "SEND", SENDERTASK_STACK_SIZE,NULL,SENDERTASK_PRIORITY,NULL);
    xTaskCreate((void *) &lifeReceiverTask, (const signed char *) "RECV", RECEIVERTASK_STACK_SIZE, NULL, RECEIVERTASK_PRIORITY, NULL);
}


// checks all the postions and the rules of the game given
void checkRules(void) {
    int gridCopy[WIDTH][HEIGHT];
    int neighbours;

    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            gridCopy[x][y] = display_message.grid[x][y];
        }
    }

    for (int x=0; x<WIDTH; x++) {
        for (int y=0; y<HEIGHT; y++) {
            int neighbours = 0; 
            for (int newX = x-1; newX <= x+1; newX++) {
                for (int newY = y-1; newY <= y+1; newY++) {  
                    if (((newX >= 0) && (newX < WIDTH)) && ((newY >= 0) && (newY<HEIGHT))) {
                        if (!((newX==x) && (newY==y))) {
                            if (gridCopy[newX][newY] == 1){
                                neighbours++; 
                            }
                        } 
                    } 
                } 
            }
            
            if (display_message.grid[x][y] == 1) {
                if (neighbours < 2) {
                    display_message.grid[x][y] = 0;
                }
                else if (neighbours == 2 || neighbours == 3) {
                    display_message.grid[x][y] = 1;
                }
                else if (neighbours > 3) {
                    display_message.grid[x][y] = 0;
                }
            }
            if (display_message.grid[x][y] == 0){
                if (neighbours == 3){
                    display_message.grid[x][y] = 1;
                }
            }
        }
    } 
} 

// sets the grid for the recevied message
void setGrid(void) {

    int type = lifeRecvMessage.type;
    int x = lifeRecvMessage.cell_x;
    int y = lifeRecvMessage.cell_y;
    if (type == 00010000) {
        display_message.grid[x][y] = 0;
    }
    else if (type == 00010001) {
        display_message.grid[x][y] = 1;       
    } 
    else if (type == 00100000) {
        display_message.grid[x][y] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+1][y+1]= 1;
    } else if (type == 00100001) {
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+2][y] = 1;
        display_message.grid[x+1][y+2] = 1;
        display_message.grid[x+2][y+2] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+3][y+1]= 1;
    } else if (type == 00100010) {
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+2][y] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+1][y+2] = 1;
        display_message.grid[x+2][y+3]= 1;
        display_message.grid[x+3][y+1]= 1;
        display_message.grid[x+3][y+2]= 1;
    
    } else if (type == 00110000) {
        display_message.grid[x][y] = 1;
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+2][y] = 1;

    } else if (type == 00110001) {
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+2][y] = 1;
        display_message.grid[x+3][y] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+1][y+1] = 1;
        display_message.grid[x+2][y+1] = 1;

    } else if (type == 00110010) {
        display_message.grid[x][y] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+1][y] = 1;
        display_message.grid[x+1][y+1]= 1;
        display_message.grid[x+2][y+2] = 1;
        display_message.grid[x+3][y+2] = 1;
        display_message.grid[x+2][y+3] = 1;
        display_message.grid[x+3][y+3]= 1;
    }
    else if (type == 01000000) {
        display_message.grid[x+2][y] = 1;
        display_message.grid[x+2][y+1] = 1;
        display_message.grid[x+2][y+2] = 1;
        display_message.grid[x][y+1] = 1;
        display_message.grid[x+1][y+2] = 1;
    }
    else if (type == 11111111) {
        pasued = 1;
    }
    else if (type == 10000000) {
        pasued = 0;
    }
    else if (type == 11000000) {
        setupGrid();
    }
}




