#include "encoder_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "timer.h"

typedef enum {IDLE, LEFT_TRANS, RIGHT_TRANS, C1, C2, C3} SM_ENCODER; //States of the machine

static pin_t rcha = PORTNUM2PIN(PD, 0);
static pin_t rchb = PORTNUM2PIN(PD, 2);
static pin_t button = PORTNUM2PIN(PD, 3);


// Timers
static tim_id_t longPressedTimerID;
static tim_id_t doublePressedTimerID;


/**
 * @brief FSM implementation
 */
static void smEnconder(void);

static encoderCb funCb;

static void cb(void);



bool encoderInit( encoderCb funCbParam ){
	timerInit();
	longPressedTimerID = timerGetId();
	doublePressedTimerID = timerGetId();
	gpioMode(rcha, INPUT);
	gpioMode(rchb, INPUT);
	gpioMode(button, INPUT);

	gpioMode(PORTNUM2PIN(PD,1), OUTPUT);

	funCb = funCbParam;

	tim_id_t periodicTimerID = timerGetId();
	timerStart(periodicTimerID, TIMER_MS2TICKS(0.5), TIM_MODE_PERIODIC, smEnconder);  // Periodic ISR 500us

	return true;
}

static void smEnconder(void){

	gpioWrite(PORTNUM2PIN(PD, 1), HIGH);

	static SM_ENCODER state_rotate = IDLE;
	static SM_ENCODER state_button = IDLE;

	switch(state_rotate){
	case IDLE:
		if( ( !gpioRead(rcha) && gpioRead(rchb) ) || ( gpioRead(rcha) && !gpioRead(rchb) )){
			if(!gpioRead(rcha))
				state_rotate = LEFT_TRANS;
			else
				state_rotate = RIGHT_TRANS;
		}
		break;
	case LEFT_TRANS:
		if(gpioRead(rcha) && gpioRead(rchb)){
			state_rotate = IDLE;
			funCb(ENC_LEFT);
		}
		break;
	case RIGHT_TRANS:
		if(gpioRead(rcha) && gpioRead(rchb)){
			state_rotate = IDLE;
			funCb(ENC_RIGHT);
		}
		break;
	}

	switch(state_button){
	case IDLE:
		if(!gpioRead(button)){
			state_button = C1;
			timerStart(longPressedTimerID, TIMER_MS2TICKS(5000), TIM_MODE_SINGLESHOT, cb);
			timerStart(doublePressedTimerID, TIMER_MS2TICKS(750), TIM_MODE_SINGLESHOT, cb);
		}
		break;
	case C1:
		if(gpioRead(button)){
			if(timerExpired(longPressedTimerID)){
				state_button = IDLE;
				funCb(ENC_LONG);
			}
			else if(timerExpired(doublePressedTimerID)){
				state_button = IDLE;
				funCb(ENC_CLICK);
			}
			else{
				state_button = C2;
				funCb(ENC_CLICK);
			}
		}
		break;
	case C2:
		if(timerExpired(doublePressedTimerID))
			state_button = IDLE;
		if(!gpioRead(button)){
			state_button = C3;
			timerStart(longPressedTimerID, TIMER_MS2TICKS(5000), TIM_MODE_SINGLESHOT, cb);
		}
		break;
	case C3:
		if(gpioRead(button)){
			if(timerExpired(longPressedTimerID)){
				state_button = IDLE;
				funCb(ENC_LONG);
			}
			else if(!timerExpired(doublePressedTimerID)){
				state_button = IDLE;
				funCb(ENC_DOUBLE);
			}
			else{
				state_button = IDLE;
				funCb(ENC_CLICK);
			}
		}
		break;
	}

	gpioWrite(PORTNUM2PIN(PD, 1), LOW);

}

static void cb(void){
}


