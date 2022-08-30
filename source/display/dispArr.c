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
#include "timer/timer.h"
#include "gpio.h"
#include <stdlib.h>
#include <string.h>

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


	// Selection (MUX) pins

#define SEL_0			PORTNUM2PIN(PC, 3)		// LSB
#define SEL_1			PORTNUM2PIN(PC, 2)		// MSB

#define SEL_ARR			{SEL_0, SEL_1}

#define SEL_PIN_COUNT	2



////// Timers

#define DISP_TIME	2	// Time of each display (milliseconds)

#define TRANS_TIME	250	// Time of digit transition (milliseconds)

#define BLINK_TIME	500	// 1/2 period of blink (milliseconds)


#define PISR_TIME	0.5	// Time of the PISR (milliseconds)


#define DISP_CYC	(DISP_TIME/PISR_TIME)

#define TRANS_CYC	(TRANS_TIME/PISR_TIME)

#define BLINK_CYC	(BLINK_TIME/PISR_TIME)


////// Brightness

#define MAX_BRIGHT	100


////// Useful macros

#define LIMIT(val, min, max)	( ( (val) >= (min) ) && ( (val) <= (max) ) ? (val) : ( (val) < (min) ) ? (min) : (max))

#define MS2CYC(ms)	((ms)/PISR_TIME)	// Converts milliseconds to PISR cycles


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef pin_t dispSegmentsPins_t[SEGMENTS_COUNT];

typedef enum {SHOW, TIMED, LOOP, ONCE, SELECT, BLINK} DISPLAY_MODES;

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

// Pin assignment
static dispSegmentsPins_t segments = SEGMENT_ARR;

static pin_t control[SEL_PIN_COUNT] = SEL_ARR;


// Display control

static dispDigit_t actualDigits[DISP_COUNT];	// Array to show on displays
static uint8_t actualDisp = 0;		// Index of array to update

static dispDigit_t selectedChar;		// Save char to blink
static uint8_t selectedIndex;

static dispDigit_t* slideDigits = NULL;	// Dynamic array to allocate digits to slide
static uint8_t slideLen;
static uint8_t slideIndex;


static DISPLAY_MODES actualMode = SHOW;


static uint8_t actualBright = MAX_BRIGHT;


// Timers

static uint32_t dispCont = DISP_CYC-1;		// Counter for display update
static uint32_t transCont = TRANS_CYC-1;	// Counter for slide
static uint32_t timedShowCont;				// Counter for TIMED mode
static uint32_t blinkCont = BLINK_CYC-1;	// Timer for BLINK mode
static uint32_t dutyCont = DISP_CYC-1;		// Counter of display show time for brightness control



// Flags

static bool blinkOff = false;
static bool dutyOff = false;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void dispPISR();

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

static void rollDisplay();

static void loopDigits();

static void string2DispArr(char* str);

// Copy the string to slideDigits after allocation
static void retrieveSlideString(char* str);


static void setMode(DISPLAY_MODES mode);

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

	// Set segments as outputs
	for (int i = 0; i < SEGMENTS_COUNT; i++) {
		gpioMode(segments[i], OUTPUT);
	}

	// Set control pins as outputs
	for (int i = 0; i < SEL_PIN_COUNT; i++) {
		gpioMode(control[i], OUTPUT);
	}

	dispArrClear();

	// Init timer for PISR
	timerInit();
	timerStart(timerGetId(), TIMER_MS2TICKS(PISR_TIME), TIM_MODE_PERIODIC, dispPISR);

	return true;

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

	setMode(SHOW);

	string2DispArr(str);

}


/**
 * @brief Show the first DISP_COUNT characters of the string given in the display for the time given
 * @param str: string to show
 * @param time: the time in milliseconds
 */
void dispArrShowForTime(char* str, uint32_t time) {

	setMode(TIMED);

	string2DispArr(str);

	timedShowCont = MS2CYC(time)-1;

}


/**
 * @brief Show the first DISP_COUNT digits of the number given in the display
 * @param num: the num to show
 */
void dispArrShowNum(uint32_t num) {

	setMode(SHOW);

	uint32_t div = 10;
	uint8_t len = 1;

	while(num/div) {
		len++;
		div *= 10;
	}

	if (len > DISP_COUNT) {
		for (int i=0; i < DISP_COUNT; i++) {
			div /= 10;
		}
		num2Digit(num/div, actualDigits);		// Keep first 4 digits, discard the rest
	}
	else {
		dispArrClear();									// Clear Array
		num2Digit(num, actualDigits+DISP_COUNT-len);	// and write to end of array
	}
}


/**
 * @brief Show the string given in the display sliding once
 * @param str: string to show
 */
void dispArrSlideOnce(char* str) {

	setMode(ONCE);

	retrieveSlideString(str);

	transCont = TRANS_CYC-1;	// Timer reset

	slideIndex = 0;		// Index reset

}


/**
 * @brief Show the string given in the display sliding in a loop
 * @param str: string to show
 */
void dispArrSlideLoop(char* str) {

	setMode(LOOP);

	retrieveSlideString(str);

	transCont = TRANS_CYC-1;	// Timer reset

	slideIndex = 0;		// Index reset
}


/**
 * @brief Show the first DISP_COUNT characters of the string given and choose one to be selected
 * @param str: string to show
 * @param sel: index of the digit to select [0 - (DISP_COUNT-1)]
 */
void dispArrShowSelect(char* str, uint8_t sel) {

	setMode(SELECT);

	string2DispArr(str);

	actualDigits[LIMIT(sel, 0, DISP_COUNT)] |= DISP_DP;		// Turn on DP on selected digit

}


/**
 * @brief Show the first DISP_COUNT characters of the string given and select once to blink periodically
 * @param str: string to show
 * @param sel: index of the digit to blink [0 - (DISP_COUNT-1)]
 */
void dispArrBlink(char* str, uint8_t sel) {

	setMode(BLINK);

	string2DispArr(str);

	selectedIndex = LIMIT(sel, 0, DISP_COUNT);

	selectedChar = actualDigits[selectedIndex];

	// Reset of timers and flags
	blinkCont = BLINK_CYC-1;
	blinkOff = false;

}


void dispArrClear() {
	for (int i = 0; i < DISP_COUNT; i++) {
		actualDigits[i] = DISP_OFF;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void dispPISR() {

	switch (actualMode) {
		case SHOW:
			// Do nothing
			break;

		case TIMED:

			if (!timedShowCont) {	// Time elapsed
				dispArrClear();			// Clear display
				actualMode = SHOW;		// and set to show mode
			}
			else {
				timedShowCont--;
			}

			break;

		case LOOP:
		case ONCE:

			if (!transCont) {
				transCont = TRANS_CYC-1;
				loopDigits();
			}
			else {
				transCont--;
			}

			break;

		case SELECT:
			// Do nothing
			break;

		case BLINK:

			if (!blinkCont) {

				actualDigits[selectedIndex] = blinkOff ? DISP_OFF : selectedChar;	// Change selected digit

				blinkOff = !blinkOff;

				blinkCont = BLINK_CYC-1;
			}
			else {
				blinkCont--;
			}
			break;

		default:
			break;
	}

	// Display rolling and brightness control.

	if (!dispCont) {
		rollDisplay();	// actualDisp increment
		dispCont = DISP_CYC-1;

		dutyCont = ((double)actualBright/MAX_BRIGHT)*DISP_CYC-1;	// Duty reset
		dutyOff = false;
	}
	else {
		dispCont--;
	}

	if (!dutyCont) {
		if (!dutyOff) {
			dispShow(segments, DISP_OFF);		// Turn off actual display
			dutyOff = true;		// Dont do it again until next display
		}
	}
	else {
		dutyCont--;
	}

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


static void loopDigits() {

	for (int i = 0; i < DISP_COUNT-1; i++) {
		actualDigits[i] = actualDigits[i+1];	// Roll
	}

	if (slideIndex < slideLen) actualDigits[DISP_COUNT-1] = slideDigits[slideIndex];	// Enter last digit and increment
	else actualDigits[DISP_COUNT-1] = DISP_OFF;

	slideIndex++;

	if (slideIndex >= slideLen+DISP_COUNT) {
		if (actualMode == ONCE) {
			actualMode = SHOW;
		}
		else if (actualMode == LOOP) {
			slideIndex = 0;
		}
		// TODO: else: throw error
	}

}

static void setMode(DISPLAY_MODES mode) {

	actualMode = mode;

}

// From string to actualDigits (clamper)
static void string2DispArr(char* str) {

	for(uint8_t i = 0; i < DISP_COUNT; i++) {

		if (*str) {
			actualDigits[i] = char2Digit(*(str++));
		}
		else {		// str length < DISP_COUNT
			actualDigits[i] = DISP_OFF;
		}

	}

}

// Copy the string to slideDigits after allocation
static void retrieveSlideString(char* str) {

	slideLen = 0;

	while(*(str + (slideLen++)));	// Get str length

	free(slideDigits);	// free before malloc

	slideDigits = (dispDigit_t*)malloc(sizeof(dispDigit_t)*(slideLen+1));	// One more for terminator

	if (slideDigits) {
		memcpy(slideDigits, str, slideLen+1);
	}
	else {
		//TODO: throw exception????
	}

}

/*******************************************************************************
 ******************************************************************************/
