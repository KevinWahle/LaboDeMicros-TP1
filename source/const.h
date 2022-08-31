/***************************************************************************//**
  @file     +const.h+
  @brief    +Constantes en comun en todo el programa+
  @author   +Grupo 3+
 ******************************************************************************/

#ifndef CONST_H
#define CONST_H

#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
//#define DEBUG 1

#define IDSIZE	8
#define PASSMIN	4
#define PASSMAX	5

enum EVENTS {
	
	// Eventos de Software
	
	//ID EVENTS
	ID_READY,
	WRONG_ID,
	ID_OK,

	// PASS EVENTS
	PASS_READY,
	WRONG_PASS,
	PASS_OK,
	CHNG_PASS,

	// USER STATE
	ADD_USER,	// evento que sale del menu
	ADD,		// evento en el que se crea el usuario
	DEL_USER,
	DELETE,

	// USER TYPES
	ADMIN_USER,
	NORMAL_USER,

	// Miscelaneos
	TIMEOUT,
	BACK,	// Para volver atras una vez checkeado cierto dato
	WAIT,

	// Evento de Hardware
	// Botones
	BTN_PRESS,
	BTN_RELEASE,
	BTN_LONG,

	// Encoder
    ENCODER_LEFT,
    ENCODER_RIGHT,
	ENCODER_PRESS,
	ENCODER_RELEASE,
	ENCODER_LONG,

	// Lector de tarjetas
	CARD_READY
};

// Identificadores para diferenciar las distintas opciones 
// seleccionadas en el menú.
enum IDS{
	CHANGE_ID,
	ADD_ID,
	DEL_ID,
	EXIT_ID,
};

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Estructura utilizada para cada una de las opciones clickeables 
// en los diferentes menúes.
typedef struct {
	char* option;
	bool admin;
	int ID;
} MENU_ITEM;


/*******************************************************************************
 ******************************************************************************/

#endif // CONST_H