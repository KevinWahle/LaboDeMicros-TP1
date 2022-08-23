/***************************************************************************//**
  @file     SysTick.c
  @brief    SysTick driver
  @author   Sergio Peralta
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "SysTick.h"
#include "hardware.h"
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

static void (*SysTickCallback)(void);

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver
 * @param funcallback Function to be call every SysTick
 * @return Initialization and registration succeed
 */
bool SysTick_Init (void (*funcallback)(void)) {

	SysTick->CTRL = 0x00;	// Disable SysTick

	SysTick->VAL = 0;		// SysTick value reset

	SysTick->LOAD = 250000 - 1;	// 2.5ms @ 100MHz			// 12500000L - 1;		// 125ms @ 100MHz

//	NVIC_SetVector(SysTick_IRQn, (uint32_t)funcallback);				// Esta bien setearlo asi? (¿alocarlo en RAM?)		///////////////////

	NVIC_EnableIRQ(SysTick_IRQn);		// Es necesario??? 			////////////

	SysTickCallback = funcallback;

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;	// Enable SysTick

	return true;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

__ISR__ SysTick_Handler (void){

	// No es necesario borrar el flag

	SysTickCallback();

}
