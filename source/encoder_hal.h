#ifndef _ENCODER_HAL_H_
#define _ENCODER_HAL_H_
#include "stdbool.h"

// Differents events supported by the encoder module
typedef enum {ENC_LEFT, ENC_RIGHT, ENC_CLICK, ENC_LONG, ENC_DOUBLE} ENC_STATE;

typedef void (*encoderCb) (ENC_STATE state);

/**
 * @brief Initializes encoder module
 * @param callback for new event notification
 */
bool encoderInit( encoderCb funCb );

#endif
