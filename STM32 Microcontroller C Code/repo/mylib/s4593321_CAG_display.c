 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_display.c
 * @author Kyle Hemmett - s4593321
 * @date 31/05/2022
 * @brief creates the display on the led
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 * 
 * display_task()
 ***************************************************************
 * 
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "oled_pixel.h"
#include "oled_string.h"
#include "fonts.h"
#include "s4593321_oled.h"
#include "s4593321_CAG_display.h"
#include "s4593321_CAG_simulator.h"
#include "debug_log.h"

// Message received 
struct Message received_message;

// Displays the grid on the screen
void display_grid(int pixels[WIDTH][HEIGHT]) {
    int x;
    int y;
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++) {
            x = i * 2;
            y = j * 2;
            if (pixels[i][j] == 1) {
                ssd1306_DrawPixel(x,y, SSD1306_WHITE);
                ssd1306_DrawPixel(x,y + 1, SSD1306_WHITE);
                ssd1306_DrawPixel(x + 1,y, SSD1306_WHITE);
                ssd1306_DrawPixel(x + 1,y + 1, SSD1306_WHITE);
            }
            else {
                ssd1306_DrawPixel(x,y, SSD1306_BLACK);
                ssd1306_DrawPixel(x,y + 1, SSD1306_BLACK);
                ssd1306_DrawPixel(x + 1,y, SSD1306_BLACK);
                ssd1306_DrawPixel(x + 1,y + 1, SSD1306_BLACK);
            }
        }
    }

    ssd1306_UpdateScreen();
}

// receives from the message Queue
void receiver_task(void) {
    // initalise oled display
    s4593321_reg_oled_init();

    for(;;) {
        if (display_queue != NULL) {
            if (xQueueReceive(display_queue, &received_message, 10)) {

                if (received_message.grid) {
                    display_grid(received_message.grid);
                }
            }
        }
        vTaskDelay(1000);
    }
}

// Display task called in main
void display_task(void) {
    xTaskCreate((void *) &receiver_task, (const signed char *) "RECV", RECEIVERTASK_STACK_SIZE, NULL, RECEIVERTASK_PRIORITY, NULL);
}
