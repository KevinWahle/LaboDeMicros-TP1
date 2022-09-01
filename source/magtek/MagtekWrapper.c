/***************************************************************************//**
  @file     +MagtekWrapper.h+
  @brief    +Mangetic stripe wrapper for 8 digit Id+
  @author   +Group 5+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

// +Incluir el header propio (ej: #include "template.h")+
#include "MagtekWrapper.h"
#include <string.h>
#include <stdlib.h>
#include "Magtek.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

# define ID_SIZE 8
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+
static void new_callback (bool state, const data* data);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static WrapperCardCb newMainCb;
static char newId[ID_SIZE+1];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
bool Card2Init(WrapperCardCb funCb){
	if (funCb==NULL)
		return false;
	CardInit(new_callback);
	newMainCb=funCb;
	return true;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void new_callback (bool state, const data* data){
	for (int i = 0; i < strlen(data->id); i++) {
	
	}
	if (state==true){
		if (strlen(data->id)>ID_SIZE){
			memcpy(newId, data->id+(strlen(data->id)-ID_SIZE)*sizeof(char),ID_SIZE);
		}
		else{
			memcpy(newId, data->id,ID_SIZE);
		}
		newId[ID_SIZE]='\0';
		if(strlen(newId)!=ID_SIZE){
			for (int i = strlen(newId); i < ID_SIZE; i++) {
				newId[i]='0';
			}
		}
		newMainCb(state, newId);
	}
	else
		newMainCb(state, NULL);
}
