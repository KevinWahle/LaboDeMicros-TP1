/***************************************************************************//**
  @file		display.c
  @brief	7 Segment Display functions
  @author	Sergio Peralta
  @date		20 ago. 2022
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "display.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static const dispDigit_t digitsNum[] = DISP_DIGITS_NUM;
static const dispDigit_t digitsAlph[] = DISP_DIGITS_ALPH;

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Get an array of digits that generate the string provided.
 * @param *s: the string.
 * @param digits: array of digits that generates s in the display (according DispDigits)
 */
void string2Digit(char* s, dispDigit_t* digits) {
	while (*s) {	// Continue until terminator
		char c = *s;

		if (c >= '0' && c <= '9') {
			*digits = digitsNum[c-'0'];
		}
		else if (c>= 'a' && c <= 'z') {
			*digits = digitsAlph[c-'a'];
		}
		else if (c >= 'A' && c <= 'Z') {
			*digits = digitsAlph[c-'A'];
		}
		else *digits = DISP_OFF;

		s++;			// Next char
		digits++;		// and next digit
	}
}


/**
 * @brief Get an array of digits that generate the number provided.
 * @param num: the number.
 * @param digits: array of digits that generates num in the display (according DispDigits)
 */
void num2Digit(uint32_t num, dispDigit_t* digits) {

	dispDigit_t* pdig = digits;

	while (num) {
		*pdig = digitsNum[num%10];	// Get the digits in reverse order

		num /= 10;
		pdig++;
	}

	while (--pdig > digits) {		// Reverse the order

		dispDigit_t temp = *pdig;

		*pdig = *digits;

		*digits = temp;

		digits++;

	}


}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



 
