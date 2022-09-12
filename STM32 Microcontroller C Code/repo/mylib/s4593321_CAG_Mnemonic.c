 /** 
 **************************************************************
 * @file mylib/s4593321_CAG_Mnemonic.c
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

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "FreeRTOS_CLI.h"
#include "s4593321_CAG_grid.h"
#include "s4593321_CAG_simulator.h"
#include "s4593321_CAG_Mnemonic.h"
#include "debug_log.h"
#include "s4593321_lta1000g.h"
#include "s4593321_user_button.h"
#include <string.h>
#include <stdlib.h>


QueueHandle_t life_queue;
struct CaMessage life_message;

static BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static BaseType_t prvStartCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static BaseType_t prvStopCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static BaseType_t prvClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );


CLI_Command_Definition_t xStill = {	// Structure that defines the "still" command line command.
	"still",							// Comamnd String
	"still: <type><x><y>\r\n",	// Help String (Displayed when "help' is typed)
	prvStillCommand,					// Command Callback that implements the command
	3								// Number of input parameters
};

CLI_Command_Definition_t xGlider = {	// Structure that defines the "glider" command line command.
	"glider",							// Comamnd String
	"glider: <x><y>\r\n",	// Help String (Displayed when "help' is typed)
	prvGliderCommand,					// Command Callback that implements the command
	2								// Number of input parameters
};


CLI_Command_Definition_t xOsc = {	// Structure that defines the "osc" command line command.
	"osc",							// Comamnd String
	"osc: <type><x><y>\r\n",	// Help String (Displayed when "help' is typed)
	prvOscCommand,					// Command Callback that implements the command
	3								// Number of input parameters
};

CLI_Command_Definition_t xStart = {	// Structure that defines the "start" command line command.
	"start",							// Comamnd String
	"start: nothing\r\n",	// Help String (Displayed when "help' is typed)
	prvStartCommand,
    0			// Command Callback that implements the command							// Number of input parameters
};

CLI_Command_Definition_t xStop = {	// Structure that defines the "stop" command line command.
	"stop",							// Comamnd String
	"stop: nothing\r\n",	// Help String (Displayed when "help' is typed)
	prvStopCommand,
    0			// Command Callback that implements the command							// Number of input parameters
};

CLI_Command_Definition_t xClear = {	// Structure that defines the "clear" command line command.
	"clear",							// Comamnd String
	"clear: nothing\r\n",	// Help String (Displayed when "help' is typed)
	prvClearCommand,
    0			// Command Callback that implements the command							// Number of input parameters
};


/*
 * Initialise CLI
 */
void Mnemonic_task(void) {

	/* Register CLI commands */
    FreeRTOS_CLIRegisterCommand(&xStill);
    FreeRTOS_CLIRegisterCommand(&xOsc);
    FreeRTOS_CLIRegisterCommand(&xGlider);
    FreeRTOS_CLIRegisterCommand(&xStart);
    FreeRTOS_CLIRegisterCommand(&xStop);
    FreeRTOS_CLIRegisterCommand(&xClear);
	/* Create CLI Receiving task */
	xTaskCreate( (void *) &cliTask, (const signed char *) "CLI", CLI_TASK_STACK_SIZE, NULL, 5, NULL );

}

/*
 * CLI Receiving Task.
 */
void cliTask(void) {

	int i;
	char cRxedChar;
	char cInputString[100];
	int InputIndex = 0;
	char *pcOutputString;
	BaseType_t xReturned;

	/* Initialise pointer to CLI output buffer. */
	memset(cInputString, 0, sizeof(cInputString));
	pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	for (;;) {
        int button_state;
        button_state = s4593321_user_button_get();
        if(button_state % 2 != 0) {
            /* Receive character from terminal */
            cRxedChar = debug_getc();

            /* Process if character if not Null */
            if (cRxedChar != '\0') {

                /* Echo character */
                debug_putc(cRxedChar);

                /* Process only if return is received. */
                if (cRxedChar == '\r') {

                    //Put new line and transmit buffer
                    debug_putc('\n');
                    debug_flush();

                    /* Put null character in command input string. */
                    cInputString[InputIndex] = '\0';

                    xReturned = pdTRUE;
                    /* Process command input string. */
                    while (xReturned != pdFALSE) {

                        /* Returns pdFALSE, when all strings have been returned */
                        xReturned = FreeRTOS_CLIProcessCommand( cInputString, pcOutputString, configCOMMAND_INT_MAX_OUTPUT_SIZE );

                        /* Display CLI command output string (not thread safe) */
                        portENTER_CRITICAL();
                        for (i = 0; i < (int) strlen(pcOutputString); i++) {
                            debug_putc(*(pcOutputString + i));
                        }
                        portEXIT_CRITICAL();

                        vTaskDelay(5);	//Must delay between debug_printfs.
                    }

                    memset(cInputString, 0, sizeof(cInputString));
                    InputIndex = 0;

                } else {

                    debug_flush();		//Transmit USB buffer

                    if( cRxedChar == '\r' ) {

                        /* Ignore the character. */
                    } else if( cRxedChar == '\b' ) {

                        /* Backspace was pressed.  Erase the last character in the
                        string - if any.*/
                        if( InputIndex > 0 ) {
                            InputIndex--;
                            cInputString[ InputIndex ] = '\0';
                        }

                    } else {

                        /* A character was entered.  Add it to the string
                        entered so far.  When a \n is entered the complete
                        string will be passed to the command interpreter. */
                        if( InputIndex < 20 ) {
                            cInputString[ InputIndex ] = cRxedChar;
                            InputIndex++;
                        }
                    }
                }
            }
        }

		vTaskDelay(50);
		
	}
}

/*
 * Still Command.
 */
static BaseType_t prvStillCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

	const char *param1, *param2, *param3;
    BaseType_t xParam1StringLength, xParam2StringLength, xParam3StringLength, xResult;

	/* Get parameters from command string */
	param1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParam1StringLength);

	param2 = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParam2StringLength);

    param3 = FreeRTOS_CLIGetParameter(pcCommandString, 3, &xParam3StringLength);

    int tp = atoi(param1);

    if (tp == 0) {
        life_message.type = 00100000;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }
    if (tp == 1) {
        life_message.type = 00100001;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }
    if (tp == 2) {
        life_message.type = 00100010;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }

	
    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}


/*
 * Osc Command.
 */
static BaseType_t prvOscCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {


	const char *param1, *param2, *param3;
    BaseType_t xParam1StringLength, xParam2StringLength, xParam3StringLength, xResult;

	/* Get parameters from command string */
	param1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParam1StringLength);

	param2 = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParam2StringLength);

    param3 = FreeRTOS_CLIGetParameter(pcCommandString, 3, &xParam3StringLength);

    int tp = atoi(param1);

    if (tp == 0) {
        life_message.type = 00110000;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }
    else if (tp == 1) {
        life_message.type = 00110001;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }
    else if (tp == 2) {
        life_message.type = 00110010;
        int x = atoi(param2);
        life_message.cell_x = x;
        int y = atoi(param3);
        life_message.cell_y = y;
    }

	/* Write command echo output string to write buffer. */
	// xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}

/*
 * Glider Command.
 */
static BaseType_t prvGliderCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {

	const char *param1, *param2;
    BaseType_t xParam1StringLength, xParam2StringLength, xResult;

	/* Get parameters from command string */
	param1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParam1StringLength);

	param2 = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParam2StringLength);

    life_message.type = 01000000;
    int x = atoi(param1);
    life_message.cell_x = x;
    int y = atoi(param2);
    life_message.cell_y = y;

	/* Write command echo output string to write buffer. */
	// xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}

/*
 * start Command.
 */
static BaseType_t prvStartCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {


    life_message.type = 11111111;
    int x = 1;
    life_message.cell_x = x;
    int y = 1;
    life_message.cell_y = y;

	/* Write command echo output string to write buffer. */
	// xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}

/*
 * stop Command.
 */
static BaseType_t prvStopCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {


    life_message.type = 10000000;
    int x = 1;
    life_message.cell_x = x;
    int y = 1;
    life_message.cell_y = y;

	/* Write command echo output string to write buffer. */
	// xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}

/*
 * clear Command.
 */
static BaseType_t prvClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ) {


    life_message.type = 11000000;
    int x = 1;
    life_message.cell_x = x;
    int y = 1;
    life_message.cell_y = y;

	/* Write command echo output string to write buffer. */
	// xWriteBufferLen = sprintf((char *) pcWriteBuffer, "\n\r%s\n\r", cCmd_string);

    xQueueSendToFront(life_queue, (void *) &life_message, (portTickType) 2);
	
    /* Return pdFALSE, as there are no more strings to return */
	/* Only return pdTRUE, if more strings need to be printed */
	return pdFALSE;
}


