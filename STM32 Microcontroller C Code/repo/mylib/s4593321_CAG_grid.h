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
#ifndef S4593321_CAG_GRID_H
#define S4593321_CAG_GRID_H

#include "event_groups.h"
#include "queue.h"

#define EVT_MOVE_UP			1 << 0		//MOVE UP
#define EVT_MOVE_LEFT		1 << 1		//MOVE DOWN
#define EVT_MOVE_DOWN		1 << 2		//MOVE UP
#define EVT_MOVE_RIGHT		1 << 3		//MOVE DOWN
#define EVT_SELECT_CELL		1 << 4		//MOVE UP
#define EVT_UNSELECT_CELL	1 << 5		//MOVE DOWN
#define EVT_TOGGLE_START	1 << 6		//MOVE UP
#define EVT_MOVE_ORIGIN		1 << 7		//MOVE DOWN
#define EVT_CLEAR_DISPLAY	1 << 8		//MOVE DOWN


#define LIFECTRL_EVENT		EVT_MOVE_UP | EVT_MOVE_LEFT | EVT_MOVE_DOWN | EVT_MOVE_RIGHT | EVT_SELECT_CELL | EVT_UNSELECT_CELL | EVT_TOGGLE_START | EVT_MOVE_ORIGIN | EVT_CLEAR_DISPLAY//Control Event Group Mask


		//Control Event Group

#define GIVETASK_PRIORITY		( tskIDLE_PRIORITY + 2 )

// Task Stack Allocations
#define GIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 4 )

#define TAKETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 4 )
#define TAKETASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

void giveTask( void );
void takeTask(void);
void grid_task(void);
int checkBounds(int xPos, int yPos);
// Task Priorities

struct CaMessage {
    int type;
    int cell_x;
    int cell_y;
}
CaMessage_t;

extern QueueHandle_t life_queue;

#endif