
/***************************************************************************//**
  @file     dispGPIO.h
  @brief    GPIO functions for 7 segment display
  @author   Sergio Peralta
 ******************************************************************************/

#ifndef DISPGPIO_H_
#define DISPGPIO_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"
#include "dispDigits.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef pin_t dispSegmentsPins_t[8];


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the segments and the cotrol pins as outputs
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN)
 * @param *control: pin array with the pins used for control (according PORTNUM2PIN)
 * @param nctrl: number of pins dedicated to control
 */
void dispArrInit(dispSegmentsPins_t segments, pin_t* control, uint8_t nctrl);


/**
 * @brief Set the control output to select the desired display. Control is assumed to be a MUX.
 * @param *control: pin array with the pins used for control (according PORTNUM2PIN and ordered from LSB to MSB)
 * @param nctrl: number of pins dedicated to control
 * @param sel: display to be selected
 */
void dispArrSelect(pin_t* control, uint8_t nctrl, uint8_t sel);


/**
 * @brief Display the desired value in the display.
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN).
 * @param digit: states of the pins of each segment in the display (according DispDigits)
 */
void dispShow(dispSegmentsPins_t segments, dispDigit_t digit);


/*******************************************************************************
 ******************************************************************************/

#endif /* DISPGPIO_H_ */
