/***************************************************************************//**
  @file     CardTest.c
  @brief    Testbench del modulo de tarjeta
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MagtekWrapper.h"
#include "board.h"
#include "gpio.h"
#include "timer/SysTick.h"
#include "hardware.h"
#include <stdio.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
WrapperCardCb mainCb;

void callback (bool state, const char* data);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    gpioMode(PIN_SW3, INPUT);					//Ya es pulldown electricamente
    gpioMode(PIN_SW2, INPUT_PULLDOWN);					//Ya es pulldown electricamente

    //SysTick_Init(callback);
   mainCb=*callback;
   Card2Init (mainCb);


}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int k;
	k++;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void callback (bool state, const char* data){
	printf("State: %d \t Id:%s\n",state, data);
}

/*******************************************************************************
 ******************************************************************************/
