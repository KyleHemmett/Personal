 /** 
 **************************************************************
 * @file mylib/s4593321_slta100g.c
 * @author Kyle Hemmett - s4593321
 * @date 03/03/2022
 * @brief led driver
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4593321_reg_lta1000g_init() - intialise 
 * s4593321_reg_lta1000g_write() - set 
 *************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"


/*
 * Sets the led bars to on or off
 */
void lta1000g_seg_set(int segment, unsigned char value) { 
  if (segment == 1 && value == 0x01) {
    GPIOC->ODR |= (0x01 << 6);   //Set the bit in the ODR
  }
  else if (segment == 1 && value == 0x00) {
    GPIOC->ODR &= ~(0x01 << 6);   //Clear the bit in the ODR
  }
  else if (segment == 2 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 15);   //Set the bit in the ODR
  }
  else if (segment == 2 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 15);   //Clear the bit in the ODR
  }
  else if (segment == 3 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 13);   //Set the bit in the ODR
  }
  else if (segment == 3 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 13);   //Clear the bit in the ODR
  }
  else if (segment == 4 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 12);   //Set the bit in the ODR
  }
  else if (segment == 4 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 12);   //Clear the bit in the ODR
  }
  else if (segment == 5 && value == 0x01) {
    GPIOA->ODR |= (0x01 << 15);   //Set the bit in the ODR
  }
  else if (segment == 5 && value == 0x00) {
    GPIOA->ODR &= ~(0x01 << 15);   //Clear the bit in the ODR
  }  
  else if (segment == 6 && value == 0x01) {
    GPIOC->ODR |= (0x01 << 7);   //Set the bit in the ODR
  }
  else if (segment == 6 && value == 0x00) {
    GPIOC->ODR &= ~(0x01 << 7);   //Clear the bit in the ODR
  }
  else if (segment == 7 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 5);   //Set the bit in the ODR
  }
  else if (segment == 7 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 5);   //Clear the bit in the ODR
  }
  else if (segment == 8 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 3);   //Set the bit in the ODR
  }
  else if (segment == 8 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 3);   //Clear the bit in the ODR
  }
  else if (segment == 9 && value == 0x01) {
    GPIOA->ODR |= (0x01 << 4);   //Set the bit in the ODR
  }
  else if (segment == 9 && value == 0x00) {
    GPIOA->ODR &= ~(0x01 << 4);   //Clear the bit in the ODR
  }
  else if (segment == 10 && value == 0x01) {
    GPIOB->ODR |= (0x01 << 4);   //Set the bit in the ODR
  }
  else if (segment == 10 && value == 0x00) {
    GPIOB->ODR &= ~(0x01 << 4);   //Clear the bit in the ODR
  }    
}

/*
 * get the value that should be displayed on the led bar
 */
void s4593321_reg_lta1000g_write(unsigned short value){
  
  int count = 0;
  while (count < 10){
    if (value & 0x01) {
      lta1000g_seg_set((count+1),0x01);
    }
    else {
      lta1000g_seg_set((count+1),0x00);
    }
    count++;
    value = value >> 1;
  }

}

/*
 * get the value that should be displayed on the led bar
 */
void s4593321_reg_lta1000g_writeX(unsigned short value){
  
  int count = 0;

  while (count < 6){
    if (value & 0x01) {
      lta1000g_seg_set((count+5),0x01);
    }
    else {
      lta1000g_seg_set((count+5),0x00);
    }
    count++;
    value = value >> 1;
  }

}

/*
 * get the value that should be displayed on the led bar
 */
void s4593321_reg_lta1000g_writeY(unsigned short value){
  
  int count = 0;
  while (count < 4){
    if (value & 0x01) {
      lta1000g_seg_set((count+1),0x01);
    }
    else {
      lta1000g_seg_set((count+1),0x00);
    }
    count++;
    value = value >> 1;
  }

}

/*
 * Initalise all variables for the led bar gpios
 */
void s4593321_reg_lta1000g_init(void) {

	// Enable the GPIO A Clock
  __GPIOA_CLK_ENABLE();
  // Enable the GPIO B Clock
  __GPIOB_CLK_ENABLE();
  // Enable the GPIO C Clock
  __GPIOC_CLK_ENABLE();


  //Initialise C6,B15,B13,B12,A15,C7,B5,B3,A4,B4 as an output.
  GPIOC->MODER &= ~(0x03 << (6 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (15 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (13 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (12 * 2));  //clear bits
  GPIOA->MODER &= ~(0x03 << (15 * 2));  //clear bits
  GPIOC->MODER &= ~(0x03 << (7 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (5 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (3 * 2));  //clear bits
  GPIOA->MODER &= ~(0x03 << (4 * 2));  //clear bits
  GPIOB->MODER &= ~(0x03 << (4 * 2));  //clear bits

  GPIOC->MODER |= (0x01 << (6 * 2));  //Set for push pull
  GPIOB->MODER |= (0x01 << (15 * 2)); //Set for push pull
  GPIOB->MODER |= (0x01 << (13 * 2)); //Set for push pull
  GPIOB->MODER |= (0x01 << (12 * 2)); //Set for push pull
  GPIOA->MODER |= (0x01 << (15 * 2)); //Set for push pull
  GPIOC->MODER |= (0x01 << (7 * 2));  //Set for push pull
  GPIOB->MODER |= (0x01 << (5 * 2));  //Set for push pull
  GPIOB->MODER |= (0x01 << (3 * 2));  //Set for push pull
  GPIOA->MODER |= (0x01 << (4 * 2));  //Set for push pull
  GPIOB->MODER |= (0x01 << (4 * 2));  //Set for push pull

  GPIOC->OSPEEDR &= ~(0x03 << (6 * 2)); 
  GPIOB->OSPEEDR &= ~(0x03 << (15 * 2)); 
  GPIOB->OSPEEDR &= ~(0x03 << (13 * 2));  
  GPIOB->OSPEEDR &= ~(0x03 << (12 * 2)); 
  GPIOA->OSPEEDR &= ~(0x03 << (15 * 2)); 
  GPIOC->OSPEEDR &= ~(0x03 << (7 * 2));  
  GPIOB->OSPEEDR &= ~(0x03 << (5 * 2));  
  GPIOB->OSPEEDR &= ~(0x03 << (3 * 2));  
  GPIOA->OSPEEDR &= ~(0x03 << (4 * 2));  
  GPIOB->OSPEEDR &= ~(0x03 << (4 * 2)); 

  GPIOC->OSPEEDR |= (0x02 << (6 * 2));  // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (15 * 2)); // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (13 * 2)); // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (12 * 2)); // Set for Fast speed
  GPIOA->OSPEEDR |= (0x02 << (15 * 2)); // Set for Fast speed
  GPIOC->OSPEEDR |= (0x02 << (7 * 2)); // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (5 * 2)); // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (3 * 2)); // Set for Fast speed
  GPIOA->OSPEEDR |= (0x02 << (4 * 2)); // Set for Fast speed
  GPIOB->OSPEEDR |= (0x02 << (4 * 2)); // Set for Fast speed

  GPIOC->OTYPER &= ~(0x01 << 6);  //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 15); //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 13); //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 12); //Clear Bit for Push/Pull output
  GPIOA->OTYPER &= ~(0x01 << 15); //Clear Bit for Push/Pull output
  GPIOC->OTYPER &= ~(0x01 << 7); //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 5); //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 3); //Clear Bit for Push/Pull output
  GPIOA->OTYPER &= ~(0x01 << 4); //Clear Bit for Push/Pull output
  GPIOB->OTYPER &= ~(0x01 << 4); //Clear Bit for Push/Pull output

  // Activate the Pull-up or Pull down resistor for the current IO
  GPIOC->PUPDR &= ~(0x03 << (6 * 2)); //Clear Bits
  GPIOB->PUPDR &= ~(0x03 << (15 * 2)); //Clear Bits
  GPIOB->PUPDR &= ~(0x03 << (13 * 2));  //Clear Bits
  GPIOB->PUPDR &= ~(0x03 << (12 * 2)); //Clear Bits
  GPIOA->PUPDR &= ~(0x03 << (15 * 2)); //Clear Bits
  GPIOC->PUPDR &= ~(0x03 << (7 * 2)); //Clear Bits 
  GPIOB->PUPDR &= ~(0x03 << (5 * 2)); //Clear Bits 
  GPIOB->PUPDR &= ~(0x03 << (3 * 2));  //Clear Bits
  GPIOA->PUPDR &= ~(0x03 << (4 * 2));  //Clear Bits
  GPIOB->PUPDR &= ~(0x03 << (4 * 2)); //Clear Bits

  GPIOC->PUPDR |= (0x01 << (6 * 2));  //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (15 * 2)); //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (13 * 2)); //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (12 * 2)); //Set for Pull down output
  GPIOA->PUPDR |= (0x01 << (15 * 2)); //Set for Pull down output
  GPIOC->PUPDR |= (0x01 << (7 * 2));  //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (5 * 2));  //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (3 * 2));  //Set for Pull down output
  GPIOA->PUPDR |= (0x01 << (4 * 2));  //Set for Pull down output
  GPIOB->PUPDR |= (0x01 << (4 * 2));  //Set for Pull down output
}