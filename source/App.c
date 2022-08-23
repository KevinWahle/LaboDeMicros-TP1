/***************************************************************************//**
  @file     App.c
  @brief    TP!: Access Control
  @author   Sergio Peralta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "SysTick.h"
#include "display/display.h"
#include "display/dispGPIO.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*************		Section Display		**************/

#define A_SEGMENT	PORTNUM2PIN(PC, 5)
#define B_SEGMENT	PORTNUM2PIN(PC, 7)
#define C_SEGMENT	PORTNUM2PIN(PC, 0)
#define D_SEGMENT	PORTNUM2PIN(PC, 9)
#define E_SEGMENT	PORTNUM2PIN(PC, 8)
#define F_SEGMENT	PORTNUM2PIN(PC, 1)
#define G_SEGMENT	PORTNUM2PIN(PB, 19)
#define DP_SEGMENT	PORTNUM2PIN(PB, 18)

#define SEGMENT_ARR	{A_SEGMENT, B_SEGMENT, C_SEGMENT, D_SEGMENT, E_SEGMENT, F_SEGMENT, G_SEGMENT, DP_SEGMENT}


#define SEL_0			PORTNUM2PIN(PC, 3)		// LSB
#define SEL_1			PORTNUM2PIN(PC, 2)		// MSB

#define SEL_ARR			{SEL_0, SEL_1}

#define SEL_PIN_COUNT	2

#define DISP_COUNT		4


/*************		Section SysTick		**************/

#define SEC_COUNT	100		// 400 -> 1 sec

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void loopLetters(uint8_t length);

void rollDisplay();

void dispSysCb();


/*******************************************************************************
 *******************************************************************************
                        	GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/

dispSegmentsPins_t segments = SEGMENT_ARR;

pin_t control[SEL_PIN_COUNT] = SEL_ARR;

dispDigit_t digitArr[ALPH_COUNT] = DISP_DIGITS_ALPH;

dispDigit_t actualDigits[DISP_COUNT];

//dispDigit_t totalDigits[] = {DISP_H, DISP_O, DISP_L, DISP_A, DISP_OFF, DISP_T, DISP_P, DISP_OFF, DISP_1};

dispDigit_t totalDigits[43];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	dispArrInit(segments, control, SEL_PIN_COUNT);
	dispArrSelect(control, SEL_PIN_COUNT, 0);
	SysTick_Init(dispSysCb);

	string2Digit("Este es el TP 1 de labo de micros", totalDigits);
	num2Digit(1234567890, totalDigits+33);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



void loopLetters(uint8_t length) {

	static uint8_t index = 0;

	for (int i = 0; i < DISP_COUNT - 1; i++) {
		actualDigits[i] = actualDigits[i+1];	// Roll
	}

	if (index < length) actualDigits[DISP_COUNT-1] = totalDigits[index];	// Enter last digit and increment
	else actualDigits[DISP_COUNT-1] = DISP_OFF;

	index++;
	index %= length+DISP_COUNT;
}

void rollDisplay() {
	static uint8_t index = 0;
	dispArrSelect(control, SEL_PIN_COUNT, index);
	dispShow(segments, actualDigits[index++]);
	index %= 4;
}

void dispSysCb() {
	static uint32_t cont = SEC_COUNT;

	rollDisplay();

	if (!cont) {
		cont = SEC_COUNT;
		loopLetters(43);
	}
	else cont--;
}

/*******************************************************************************
 ******************************************************************************/
