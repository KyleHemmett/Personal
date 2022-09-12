#include "board.h"
#include "processor_hal.h"
#include "s4593321_lta1000g.h"
#include "s4593321_joystick.h"
#include "s4593321_pantilt.h"
#include "debug_log.h"

int main(void)  {

  HAL_Init();			  //Initialise board.
	s4593321_reg_lta1000g_init();	//Initialise hardware modules
  s4593321_reg_joystick_pb_init();
  s4593321_reg_pantilt_init();
  BRD_LEDInit();
  BRD_debuguart_init();

	// Main Processing Loop
	while (1)
	{

    unsigned int adc_x = S4593321_REG_JOYSTICK_X_READ();
    unsigned int adc_y = S4593321_REG_JOYSTICK_Y_READ();

    int xvalue = ((adc_x*(float)0.044) - 92);
    int yvalue = ((adc_y*(float)0.044) - 90);

		// If ADC value is higher than 1/2 Reference voltage, turn on Green LED.
		// Else if ADC value is lower than 1/2 Reference voltage, turn off Green LED.


		// if (adc_x > 0x7FF) {
	
		// 	BRD_LEDGreenOn();
		// } else {

		// 	BRD_LEDGreenOff();
		// }

    // if (adc_y > 0x7FF) {
    //   BRD_LEDRedOff();
    // } else {
    //   BRD_LEDRedOn();
    // }

    debug_log("%d, %d\n\r", xvalue, yvalue);
    HAL_Delay(50);
    //s4593321_reg_lta1000g_write(adc_x);
    S4593321_REG_PANTILT_PAN_WRITE(xvalue);
    S4593321_REG_PANTILT_TILT_WRITE(yvalue);
    HAL_Delay(10);
    //float val = S4593321_REG_PANTILT_PAN_READ();
    //debug_log("%f\n\r", val);
    // HAL_Delay(3000);
    // S4593321_REG_PANTILT_PAN_WRITE(-90);
    // val = S4593321_REG_PANTILT_PAN_READ();
    // debug_log("%f\n\r", val);
    // HAL_Delay(3000);

    //s4593321_pantilt_angle_write(1,90);
    //s4593321_pantilt_angle_write(0,-90);
    //HAL_Delay(1000);

	}


  return 0;

}