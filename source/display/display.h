/***************************************************************************//**
  @file     display.h
  @brief    7 Segment Display functions
  @author   Sergio Peralta
  @date		20 ago. 2022
 ******************************************************************************/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dispDigits.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Get an array of digits that generate the string provided.
 * @param *s: the string.
 * @param digits: array of digits that generates s in the display (according DispDigits)
 */
void string2Digit(char* s, dispDigit_t* digits);


/**
 * @brief Get an array of digits that generate the number provided.
 * @param num: the number.
 * @param digits: array of digits that generates num in the display (according DispDigits)
 */
void num2Digit(uint32_t num, dispDigit_t* digits);


/*******************************************************************************
 ******************************************************************************/

#endif // _DISPLAY_H_
