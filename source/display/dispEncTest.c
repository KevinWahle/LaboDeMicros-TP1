/***************************************************************************//**
  @file		dispTest.c
  @brieef	Testbench para las funciones de dispArr
  @author	Sergio Peralta
  @date		30 ago. 2022
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "dispArr.h"
#include "encoder_hal.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void nextMode(ENC_STATE state);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	dispArrInit();
	encoderInit(nextMode);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{



}

void nextMode(ENC_STATE state) {

	static uint8_t mode = 0;

	if (state == ENC_RIGHT || state == ENC_CLICK) {
		mode++;
	}
	else if (state == ENC_LEFT || state == ENC_LONG) {
		mode--;
	}
	else if (state == ENC_DOUBLE) {
		mode = 0;
	}

	switch (mode) {

	case 0:
		dispArrShow("Hola");
		break;

	case 1:
		dispArrShow("Mensaje largo");
		break;

	case 2:
		dispArrShowForTime("Hola", 5000);
		break;

	case 3:
		dispArrShowForTime("Mensaje largo", 3500);
		break;

	case 4:
		dispArrShowNum(12);
		break;

	case 5:
		dispArrShowNum(1234);
		break;

	case 6:
		dispArrShowNum(56789012);
		break;

	case 7:
		dispArrSlideOnce("Un texto bastante largo");
		break;

	case 8:
		dispArrSlideOnce("Hola");
		break;

	case 9:
		dispArrSlideLoop("Un texto bastante largo");
		break;

	case 10:
		dispArrSlideLoop("Hola");
		break;

	case 11:
	case 12:
	case 13:
	case 14:
		dispArrShowSelect("Hola", mode-11);
		break;

	case 15:
		dispArrShowSelect("Hola", 4);
		break;

	case 16:
	case 17:
	case 18:
	case 19:
		dispArrShowSelect("Mensaje largo", mode-16);
		break;

	case 20:
		dispArrShowSelect("Mensaje largo", 5);
		break;

	case 21:
	case 22:
	case 23:
	case 24:
		dispArrBlink("Hola", mode-21);
		break;

	case 25:
		dispArrBlink("Hola", 4);
		break;

	case 26:
	case 27:
	case 28:
	case 29:
		dispArrBlink("Mensaje largo", mode-26);
		break;

	case 30:
		dispArrBlink("Mensaje largo", 10);
		break;

	case 31:
		dispArrClear();		// Hay que cambiar a modo show
		break;

	}

	if (mode > 31) {
		dispArrSetBright(mode-35);
		dispArrShowNum(dispArrGetBright());
	}

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



 
