#include "board.h"
#include "processor_hal.h"
#include "/home/csse3010/csse3010/repo/mylib/s4593321_lta1000g.h"
#include "/home/csse3010/csse3010/repo/mylib/s4593321_joystick.h"

int main(void)  {

  HAL_Init();			  //Initialise board.
	s4593321_reg_lta1000g_init();	//Initialise hardware modules
  s4593321_reg_joystick_pb_init();

    while(1){
      s4593321_reg_lta1000g_write(s4593321_reg_joystick_press_get());

      
    }      

  return 0;
}