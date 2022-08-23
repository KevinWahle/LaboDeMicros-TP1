/*
 * dispDigits.h
 *
 *  Created on: 20 ago. 2022
 *      Author: Sergio Peralta
 */

#ifndef DISPDIGITS_H_
#define DISPDIGITS_H_

#include <stdint.h>

typedef uint8_t dispDigit_t;

//					  abcdefgDP		ON: 1  OFF:0

#define DISP_OFF	0b00000000

#define DISP_DP		0b00000001

#define DISP_0		0b11111100

#define DISP_1		0b01100000

#define DISP_2		0b11011010

#define DISP_3		0b11110010

#define DISP_4		0b01100110

#define DISP_5		0b10110110

#define DISP_6		0b10111110

#define DISP_7		0b11100000

#define DISP_8		0b11111110

#define DISP_9		0b11100110

#define DISP_A		0b11101110

#define DISP_B		0b00111110

#define DISP_C		0b10011100

#define DISP_D		0b01111010

#define DISP_E		0b10011110

#define DISP_F		0b10001110

#define DISP_G		0b10111100

#define DISP_H		0b01101110	// h:0b00101110

#define DISP_I		DISP_1

#define DISP_J		0b01110000

#define DISP_K	0b00000000		//////////////////////////

#define DISP_L		0b00011100

#define DISP_M		DISP_H	//0b10101000

#define DISP_N		0b00101010

#define DISP_O		0b00111010	//(o)  O: DISP_0

#define DISP_P		0b11001110

#define DISP_Q		0b11100110

#define DISP_R		0b00001010

#define DISP_S		DISP_5		//0b10110110

#define DISP_T		0b00011110

#define DISP_U		0b01111100

#define DISP_V		0b00000000		//////////////////////////

#define DISP_W		0b00000000		//////////////////////////

#define DISP_X		0b00000000		//////////////////////////

#define DISP_Y		0b01110110

#define DISP_Z		DISP_2

#define DISP_DIGITS_NUM		{DISP_0, DISP_1, DISP_2, DISP_3, DISP_4, DISP_5, DISP_6, DISP_7, DISP_8, DISP_9}

#define DISP_DIGITS_ALPH {DISP_A, DISP_B, DISP_C, DISP_D, DISP_E, DISP_F, DISP_G, DISP_H, DISP_I, DISP_J, DISP_K, DISP_L, DISP_M, DISP_N, DISP_O, DISP_P, DISP_Q, DISP_R, DISP_S, DISP_T, DISP_U, DISP_V, DISP_W, DISP_X, DISP_Y, DISP_Z}

#define ALPH_COUNT	26

#endif /* DISPDIGITS_H_ */
