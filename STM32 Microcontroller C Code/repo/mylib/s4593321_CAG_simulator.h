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
#ifndef S4593321_CAG_SIMULATOR_H
#define S4593321_CAG_SIMULATOR_H

#include "queue.h"
#include "event_groups.h"

#define I2C_DEV_SDA_PIN		9
#define I2C_DEV_SCL_PIN		8
#define I2C_DEV_GPIO		GPIOB
#define I2C_DEV_GPIO_AF 	GPIO_AF4_I2C1
#define I2C_DEV_GPIO_CLK()	__GPIOB_CLK_ENABLE()

#define I2C_DEV				I2C1
#define I2C_DEV_CLOCKSPEED 	100000

#define SENDERTASK_PRIORITY     (tskIDLE_PRIORITY + 2)
#define SENDERTASK_STACK_SIZE   (configMINIMAL_STACK_SIZE * 2)

#define WIDTH 64
#define HEIGHT 16

void setupGrid(void);
void senderTask(void);
void simulator_task(void);
void checkRules(void);
void lifeReceiverTask(void);
void setGrid(void);

struct Message {
    int grid[WIDTH][HEIGHT];
}
message_t;

extern QueueHandle_t display_queue;

extern EventGroupHandle_t lifectrlEventGroup;


#endif