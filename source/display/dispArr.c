/***************************************************************************//**
  @file     dispArr.c
  @brief    Functions for 7 segment display array
  @author   Sergio Peralta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dispArr.h"
#include "dispDigits.h"
#include "gpio.h"
#include "SysTick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


////// Pin Assignment

	// Segments

#define A_SEGMENT	PORTNUM2PIN(PC, 5)
#define B_SEGMENT	PORTNUM2PIN(PC, 7)
#define C_SEGMENT	PORTNUM2PIN(PC, 0)
#define D_SEGMENT	PORTNUM2PIN(PC, 9)
#define E_SEGMENT	PORTNUM2PIN(PC, 8)
#define F_SEGMENT	PORTNUM2PIN(PC, 1)
#define G_SEGMENT	PORTNUM2PIN(PB, 19)
#define DP_SEGMENT	PORTNUM2PIN(PB, 18)

#define SEGMENT_ARR	{A_SEGMENT, B_SEGMENT, C_SEGMENT, D_SEGMENT, E_SEGMENT, F_SEGMENT, G_SEGMENT, DP_SEGMENT}

#define SEGMENTS_COUNT	8

	// Selection (MUX) pins

#define SEL_0			PORTNUM2PIN(PC, 3)		// LSB
#define SEL_1			PORTNUM2PIN(PC, 2)		// MSB

#define SEL_ARR			{SEL_0, SEL_1}

#define SEL_PIN_COUNT	2



////// Timers

#define DISP_TIME	2	// Time of each display (milliseconds)

#define TRANS_TIME	250	// Time of digit transition (milliseconds)

#define BLINK_TIME	500	// 1/2 period of blink (milliseconds)


#define PISR_TIME	1	// Time of the PISR (milliseconds)


#define DISP_CYC	(DISP_TIME/PISR_TIME)

#define TRANS_CYC	(TRANS_TIME/PISR_TIME)

#define BLINK_CYC	(BLINK_TIME/PISR_TIME)


////// Brightness

#define MAX_BRIGHT	100


////// Useful macros

#define LIMIT(val, min, max)	( ( (val) >= (min) ) && ( (val) <= (max) ) ? (val) : ( (val) < (min) ) ? (min) : (max))



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef pin_t dispSegmentsPins_t[SEGMENTS_COUNT];

enum DISPLAY_MODES {STAND_BY, SHOW, TIMED, LOOP, ONCE};

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

static dispSegmentsPins_t segments = SEGMENT_ARR;

static pin_t control[SEL_PIN_COUNT] = SEL_ARR;

//static dispDigit_t digitArr[ALPH_COUNT] = DISP_DIGITS_ALPH;

static dispDigit_t actualDigits[DISP_COUNT];

static dispDigit_t actualChar;

static DISPLAY_MODES actualMode = STAND_BY;


//static dispDigit_t totalDigits[] = {DISP_H, DISP_O, DISP_L, DISP_A, DISP_OFF, DISP_T, DISP_P, DISP_OFF, DISP_1};

static dispDigit_t totalDigits[43];


static uint8_t actualDisp = 0;

static uint8_t actualBright = MAX_BRIGHT;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Set the control output to select the desired display. Control is assumed to be a MUX.
 * @param *control: pin array with the pins used for control (according PORTNUM2PIN and ordered from LSB to MSB)
 * @param nctrl: number of pins dedicated to control
 * @param sel: display to be selected
 */
static void dispArrSelect(uint8_t sel);


/**
 * @brief Display the desired value in the display.
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN).
 * @param digit: states of the pins of each segment in the display (according DispDigits)
 */
static void dispShow(dispSegmentsPins_t segments, dispDigit_t digit);

static void dispPISR();

static void rollDisplay();

static void loopLetters(uint8_t length);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Configures the 7 Segment Display Array Driver
 * @return Initialization succeed
 */
bool dispArrInit() {

	for (int i = 0; i < SEGMENTS_COUNT; i++) {
		gpioMode(segments[i], OUTPUT);
	}

	for (int i = 0; i < SEL_PIN_COUNT; i++) {
		gpioMode(control[i], OUTPUT);
	}



/////////////////  TEST  /////////////////////////
	string2Digit("Este es el TP 1 de labo de micros", totalDigits);
	num2Digit(1234567890, totalDigits+33);

	//TODO: Activar interrupciones periodicas con timer
	return SysTick_Init(dispPISR);

}


/**
 * @brief Set the brightness of the display array
 * @param bright: Level of brightness [0-100]
 */
void dispSetBright(uint8_t bright) {
	actualBright = LIMIT(bright, 0, MAX_BRIGHT);
}


/**
 * @brief Show the first DISP_COUNT characters of the string given in the display
 * @param str: string to show
 */
void dispArrShow(char* str) {

}


/**
 * @brief Show the first DISP_COUNT characters of the string given in the display for the time given
 * @param str: string to show
 * @param time: the time in milliseconds
 */
void dispArrShowForTime(char* str, uint32_t time) {

}


/**
 * @brief Show the first DISP_COUNT digits of the number given in the display
 * @param num: the num to show
 */
void dispArrShowNum(uit32_t num) {

}


/**
 * @brief Show the string given in the display sliding once
 * @param str: string to show
 */
void dispArrSlideOnce(char* str) {

}


/**
 * @brief Show the string given in the display sliding in a loop
 * @param str: string to show
 */
void dispArrSlideLoop(char* str) {

}


/**
 * @brief Show the first DISP_COUNT characters of the string given and select once to blink periodically
 * @param str: string to show
 * @param sel: index of the digit to blink [0 - (DISP_COUNT-1)]
 */
void dispArrBlink(char* str, uint8_t sel) {

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void dispPISR() {

	//TODO: Chequear esto

	static uint32_t dispCont = DISP_CYC;
	static uint32_t transCont = TRANS_CYC;
	static uint32_t blinkCont = BLINK_CYC;
	static uint32_t dutyCont = ((double)(actualBright/MAX_BRIGHT))*DISP_CYC;	// Counter of display show time

	if (!dutyCont) {
		if (actualDigits[actualDisp] != DISP_OFF) {
			dispShow(DISP_OFF, actualDisp);
		}
	}
	else {
		dutyCont--;
	}

	if (!dispCont) {
		rollDisplay();
		dispCont = DISP_CYC;
		dutyCont = ((double)(actualBright/MAX_BRIGHT))*DISP_CYC;	// Duty reset
	}

	switch (actualMode) {
		case STAND_BY:
			break;

		case SHOW:
			break;

		case TIMED:
			break;

		case LOOP:

			if (!transCont) {
				transCont = TRANS_CYC;
				loopLetters(43);
			}
			else {
				transCont--;
				//TODO: Reset si se cambia de modo o llama de vuelta
			}

			break;

		case ONCE:
			break;

		default:
			break;
	}


	dispCont--;
//	blinkCont--;

}

/**
 * @brief Set the control output to select the desired display. Control is assumed to be a MUX.
 * @param sel: display to be selected
 */
static void dispArrSelect(uint8_t sel) {

	for (int i = 0; i < SEL_PIN_COUNT; i++) {
		gpioWrite(control[i], sel & 0x1);		// Write last bit
		sel >>= 1;								// Next bit
	}

}


/**
 * @brief Display the desired value in the display.
 * @param segments[8]: pin array with the pins of the segments (according PORTNUM2PIN).
 * @param digit: states of the pins of each segment in the display (according DispDigits)
 */
static void dispShow(dispSegmentsPins_t segments, dispDigit_t digit) {

	for (int i = (SEGMENTS_COUNT-1); i >= 0; i--) {		// Reverse order (MSB is a)
		gpioWrite(segments[i], digit & 0x1);	// Write bit
		digit >>= 1;							// Next bit
	}

}


static void rollDisplay() {
	dispArrSelect(actualDisp);							// Select next display
	dispShow(segments, actualDigits[actualDisp++]);		// and show next char
	actualDisp %= DISP_COUNT;							// increment counter and reset
}


void loopLetters(uint8_t length) {

	static uint8_t index = 0;

	for (int i = 0; i < DISP_COUNT-1; i++) {
		actualDigits[i] = actualDigits[i+1];	// Roll
	}

	if (index < length) actualDigits[DISP_COUNT-1] = totalDigits[index];	// Enter last digit and increment
	else actualDigits[DISP_COUNT-1] = DISP_OFF;

	index++;
	index %= length+DISP_COUNT;
}

/*******************************************************************************
 ******************************************************************************/
