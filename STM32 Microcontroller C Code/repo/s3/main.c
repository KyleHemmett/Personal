#include "board.h"
#include "processor_hal.h"
#include "s4593321_lta1000g.h"
#include "s4593321_joystick.h"
#include "s4593321_pantilt.h"
#include "s4593321_hamming.h"
#include "debug_log.h"
#include <stdio.h>
#include <stdlib.h>

// States used
#define S0  0
#define S1  1
#define S2  2
#define S3  3
#define S4  4

int fsm_processing(int current, unsigned char input_one, unsigned char input_two);
void show_encode(unsigned upper_value, unsigned lower_value);
void show_decode(unsigned value);

/*
 * Main program
 */
int main(void)  {

  uint32_t prev_tick;
  int currentState = S0;


  HAL_Init();
	s4593321_reg_lta1000g_init();
  s4593321_reg_joystick_pb_init();
  BRD_LEDInit();
  BRD_debuguart_init();
	
  prev_tick = 0;
  unsigned int first = 0;
  unsigned char input_one;
  unsigned char input_two;
  int prev_button_state;
  unsigned char prev_input_one;
  unsigned char prev_input_two; 

  while (1) {

    if ((HAL_GetTick() - prev_tick) >= 50) {
      int button_state = s4593321_reg_joystick_press_get();

      if (first == 0){
        input_one = BRD_debuguart_getc();
        if (input_one == '\0' || input_one == ' '){
          if(button_state != prev_button_state){
            prev_button_state = s4593321_reg_joystick_press_get();
            currentState = fsm_processing(currentState, prev_input_one, prev_input_two);
          }
          prev_tick =  HAL_GetTick();
          continue;
        }
        else{
          debug_log("value 1 = %c\n\r", input_one);
          first = 1;
        }
      }
      else if(first == 1){
        input_two = BRD_debuguart_getc();
        if (input_two == '\0' || input_two ==' '){
          if(button_state != prev_button_state){
            prev_button_state = s4593321_reg_joystick_press_get();
            currentState = fsm_processing(currentState, prev_input_one, prev_input_two);
          }
          prev_tick =  HAL_GetTick();
          continue;
        }
        else{
          debug_log("value 2 = %c\n\r", input_two);
          first = 0;
        }
      }
      currentState = fsm_processing(currentState, input_one, input_two);
      prev_tick =  HAL_GetTick();
      prev_button_state = s4593321_reg_joystick_press_get();
      prev_input_one = input_one;
      prev_input_two = input_two;
    }
  }

  return 0;
}

/*
 * Return current state and process outputs
 */
int fsm_processing(int current, unsigned char input_one, unsigned char input_two) {

  int nextState = S0;

  switch(current) {

    case S0:  

      if (input_one == 'E' && input_two == 'E'){
        nextState = S1;
      }
      else if (input_one == 'D' && input_two == 'D'){
        nextState = S2;
      }   
      else{
        nextState = S0;
      }
      break;

    case S1:

      if (input_one == 'F' && input_two == 'F'){
        nextState = S0;
      }
      else{
        char buffer1[8];
        uint8_t buffer2;
        sprintf(buffer1, "%c%c",input_one, input_two);
        int num = (int)strtol(buffer1, NULL, 16);
        unsigned short out = s4593321_lib_hamming_byte_encode(num);
        
        unsigned upper_value = ((out >> 8));

        unsigned	lower_value = (out & 0xFF);
        show_encode(upper_value,lower_value);
        nextState = S1;
      }
      break;                                

    case S2:
      if (input_one == 'F' && input_two == 'F'){
        nextState = S0;
      } 
      else{
        char buffer1[8];
        uint8_t buffer2;
        sprintf(buffer1, "%c%c",input_one, input_two);
        int num = (int)strtol(buffer1, NULL, 16);
        unsigned short out = s4593321_lib_hamming_byte_decode(num);
        show_decode(out);
        nextState = S2;        
      }
      break;
  }

  return nextState;
}

void show_encode(unsigned upper_value, unsigned lower_value){
  
  int button_value = s4593321_reg_joystick_press_get();


  if(button_value%2 == 0){
    debug_log("upper %X\n\r", upper_value);
    s4593321_reg_lta1000g_write(upper_value);
  }
  else{
    debug_log("lower %X\n\r", lower_value);
    s4593321_reg_lta1000g_write(lower_value);
  }

}

void show_decode(unsigned value){
  debug_log("decode %X\n\r", value);
  int par = s4593321_lib_parity_error(value);

  s4593321_reg_lta1000g_write(value);

  if (par == 1){
    lta1000g_seg_set(8,1);
    lta1000g_seg_set(9,1);
  }
  else{
    lta1000g_seg_set(8,0);
    lta1000g_seg_set(9,0);    
  }

}


