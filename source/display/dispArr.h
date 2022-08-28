
/***************************************************************************//**
  @file     dispArr.h
  @brief    Functions for 7 segment display array
  @author   Sergio Peralta
 ******************************************************************************/

#ifndef _DISPARR_H_
#define _DISPARR_H_


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DISP_COUNT		4

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the 7 Segment Display Array Driver
 * @return Initialization succeed
 */
bool dispArrInit();


/**
 * @brief Set the brightness of the display array
 * @param bright: Level of brightness [0-100]
 */
void dispSetBright(uint8_t bright);


/**
 * @brief Show the first DISP_COUNT characters of the string given in the display
 * @param str: string to show
 */
void dispArrShow(char* str);


/**
 * @brief Show the first DISP_COUNT characters of the string given in the display for the time given
 * @param str: string to show
 * @param time: the time in milliseconds
 */
void dispArrShowForTime(char* str, uint32_t time);


/**
 * @brief Show the first DISP_COUNT digits of the number given in the display
 * @param num: the num to show
 */
void dispArrShowNum(uit32_t num);


/**
 * @brief Show the string given in the display sliding once
 * @param str: string to show
 */
void dispArrSlideOnce(char* str);


/**
 * @brief Show the string given in the display sliding in a loop
 * @param str: string to show
 */
void dispArrSlideLoop(char* str);


/**
 * @brief Show the first DISP_COUNT characters of the string given and select once to blink periodically
 * @param str: string to show
 * @param sel: index of the digit to blink [0 - (DISP_COUNT-1)]
 */
void dispArrBlink(char* str, uint8_t sel);

/*******************************************************************************
 ******************************************************************************/

#endif /* _DISPARR_H_ */
