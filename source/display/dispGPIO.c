/***************************************************************************//**
  @file     dispGPIO.c
  @brief    GPIO functions for 7 segment display
  @author   Sergio Peralta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dispGPIO.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Configures the segments and the cotrol pins as outputs
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN)
 * @param control: pin array with the pins used for control (according PORTNUM2PIN)
 * @param nctrl: number of pins dedicated to control
 */
void dispArrInit(dispSegmentsPins_t segments, pin_t* control, uint8_t nctrl) {

	for (int i = 0; i < 8; i++) {
		gpioMode(segments[i], OUTPUT);
	}

	for (int i = 0; i < nctrl; i++) {
		gpioMode(control[i], OUTPUT);
	}

}

/**
 * @brief Set the control output to select the desired display. Control is assumed to be a MUX.
 * @param *control: pin array with the pins used for control (according PORTNUM2PIN and ordered from LSB to MSB)
 * @param nctrl: number of pins dedicated to control
 * @param sel: display to be selected
 */
void dispArrSelect(pin_t* control, uint8_t nctrl, uint8_t sel) {

	for (int i = 0; i < nctrl; i++) {
		gpioWrite(control[i], sel & 0x1);		// Write last bit
		sel >>= 1;								// Next bit
	}

}


/**
 * @brief Display the desired value in the display.
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN).
 * @param digit: states of the pins of each segment in the display (according DispDigits)
 */
void dispShow(dispSegmentsPins_t segments, dispDigit_t digit) {

	for (int i = 7; i >= 0; i--) {		// Reverse order (MSB is a)
		gpioWrite(segments[i], digit & 0x1);	// Write bit
		digit >>= 1;							// Next bit
	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
