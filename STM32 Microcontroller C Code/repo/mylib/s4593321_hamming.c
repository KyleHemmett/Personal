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

#include "board.h"
#include "processor_hal.h"
#include "s4593321_hamming.h"

unsigned short s4593321_lib_hamming_byte_encode(unsigned char value){
	uint16_t out;

	/* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */
	out = hamming_hbyte_encode(value & 0xF) |
		(hamming_hbyte_encode(value >> 4) << 8);

	return(out);

}

unsigned short hamming_hbyte_encode(unsigned char value){
	uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;

	/* extract bits */
	d0 = !!(value & 0x1);
	d1 = !!(value & 0x2);
	d2 = !!(value & 0x4);
	d3 = !!(value & 0x8);

	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;

	/* generate out byte without parity bit P0 */
	out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
		(d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

	/* calculate even parity bit */
	for (z = 1; z<8; z++)
		p0 = p0 ^ !!(out & (1 << z));

	out |= p0;

	return(out);
}


unsigned short s4593321_lib_hamming_byte_decode(unsigned char value){
	uint8_t d3, d2, d1, d0;
	uint8_t h2, h1, h0, p0;
	uint8_t out;

	d0 = !!(value & 0x10);
	d1 = !!(value & 0x20);
	d2 = !!(value & 0x40);
	d3 = !!(value & 0x80);

	out = (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3);
	return(out);

}


unsigned short s4593321_lib_parity_error(unsigned char value){

	int i = 0;
	int ones = 0;

	while(value > 0){
		ones++;
		value = value & (value - 1);
	}

	if(ones %2 == 0){
		return(0);
	}
	else{
		return(1);
	}
}

