 /** 
 **************************************************************
 * @file mylib/s4593321_hamming.c
 * @author Kyle Hemmett - s4593321
 * @date 11/04/2022
 * @brief Hamming
 * REFERENCE: -.pdf 
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * 
 *************************************************************** 
 */
#ifndef S4593321_HAMMING_H
#define S4593321_HAMMING_H

unsigned short s4593321_lib_hamming_byte_encode(unsigned char value);
unsigned short hamming_hbyte_encode(unsigned char value);
unsigned short s4593321_lib_hamming_byte_decode(unsigned char value);
unsigned short s4593321_lib_parity_error(unsigned char value);


#endif