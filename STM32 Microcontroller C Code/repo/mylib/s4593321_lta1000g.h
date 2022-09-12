 /** 
 **************************************************************
 * @file mylib/s4593321_slta100g.h
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

#ifndef S4593321_LTA1000G_H
#define S4593321_LTA1000G_H

void s4593321_reg_lta1000g_init(void);
void lta1000g_seg_set(int segment, unsigned char value);
void s4593321_reg_lta1000g_write(unsigned short value);
void s4593321_reg_lta1000g_writeX(unsigned short value);
void s4593321_reg_lta1000g_writeY(unsigned short value);

#endif