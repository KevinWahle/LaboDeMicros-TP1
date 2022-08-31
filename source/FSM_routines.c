/***************************************************************************//**
  @file     +FSM_routines.c+
  @brief    +Funciones que conforman las rutas de acción entre los diferentes
            estados de la FSM.+
  @author   +Grupo 3+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSM_routines.h"
#include "event_queue/event_queue.h"
#include "encdec.h"
#include "const.h"
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
static uint8_t actual_id[IDSIZE]={NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR};
static uint8_t actual_pass[PASSMAX]={NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR};
static uint8_t digitCounter;

MENU_ITEM admin_menu[] = {  
                            {.option = "CHANGE", .ID = CHANGE_ID},
                            {.option = "ADD", .ID = ADD_ID},
                            {.option = "DELETE", .ID = DEL_ID},
                            {.option = "BACK", .ID = EXIT_ID},
                        };

MENU_ITEM user_menu[] = {  
                            {.option = "CHANGE", .ID = CHANGE_ID},
                            {.option = "BACK", .ID = EXIT_ID},
                        };


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t actual_option = 0;           // Variable que marca la opcion del menú seleccionada.     

void update_display(uint8_t* arr, uint8_t counter, bool password);
void updateMenuDis(char* word);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**********************************************************
************************  ID  ***************************
**********************************************************/
void id_init(vod){
    for(int i=0; i<IDSIZE; i++)
    {
        actual_id[i]=NULLCHAR;
    }
    digitCounter=0;
    update_display(actual_id, digitCounter, 0);
}

void previous_id(void){
    if(digitCounter>0){
        digitCounter--;
        update_display(actual_id, digitCounter, 0);
    }
}

void upper_id(){
    if (actual_id[digitCounter]<9)
    {
        actual_id[digitCounter]++;
    }
    else if(actual_id[digitCounter]==NULLCHAR)
        actual_id[digitCounter]=0;
    else
        actual_id[digitCounter]=0;

    update_display(actual_id, digitCounter, 0);        
}

void next_id(){
    if(digitCounter<IDSIZE){
        digitCounter++;
    }

    else{
        add_event(ID_READY);
    }

    update_display(actual_id, digitCounter, 0);
}

/**********************************************************
************************  PASSWORD  ***********************
**********************************************************/

void pass_init(){
    for(int i=0; i<PASSMAX; i++)
    {
        actual_pass[i]=NULL;
    }
    digitCounter=0;

    update_display(actual_pass, digitCounter, 1);
}

void previous_pass(){
    if(digitCounter>0){
        digitCounter--;
        update_display(actual_pass, digitCounter, 1);
    }
    else{
        add_event(BACK);
    }
}

void upper_pass(){
    if (actual_pass[digitCounter]<9)
        actual_pass[digitCounter]++;
    else if(actual_id[digitCounter]==NULLCHAR)
        actual_id[digitCounter]=0;
    else
        actual_pass[digitCounter]=0;

    update_display(actual_pass, digitCounter, 1);
}

void next_pass(){
    if(digitCounter<PASSMAX){
        digitCounter++;
    }

    update_display(actual_pass, digitCounter, 1);
}

/**********************************************************
************************  MENU  ***************************
**********************************************************/

void init_admin_menu(){
    actual_option=0;
    updateMenuDis(admin_menu[actual_option].option);
}

void up_menu_Admin(){
    if (actual_option<ADMIN_MENU_LEN){
        actual_option++;
        updateMenuDis(admin_menu[actual_option].option);
    }
}

void down_menu_Admin(){
    if (actual_option>0){
        actual_option--;
        updateMenuDis(admin_menu[actual_option].option);
    }
}

void click_menu_Admin(){
    switch (admin_menu[actual_option].ID){
        case CHANGE_ID:
            add_event(CHNG_PASS);
            break;

        case ADD_ID:
            add_event(ADD_USER);
            break;

        case DEL_ID:
            add_event(DEL_USER);
            break;

        case EXIT_ID:
            add_event(BACK);
            break;
        
        default:
            break;
    }
}

void init_menu(){
    actual_option=0;
    updateMenuDis(user_menu[actual_option].option);
}

void down_menu(){
    if (actual_option<USER_MENU_LEN){
        actual_option++;
        updateMenuDis(user_menu[actual_option].option);
    }
}

void up_menu(){
    if (actual_option>0){
        actual_option--;
        updateMenuDis(user_menu[actual_option].option);
    }
}

void click_menu(){
    switch (user_menu[actual_option].ID){
        case CHANGE_ID:
            add_event(CHNG_PASS);
            break;

        case EXIT_ID:
            add_event(BACK);
            break;
        
        default:
            break;
    }
}

/**********************************************************
********************  CALLBACKS  *************************
**********************************************************/

//TODO: Agregar al .h
void encoderCallback(ENC_STATE state){
    switch(state){
        case ENC_LEFT:
            add_event(ENCODER_LEFT);
        break;

        case ENC_RIGHT:
            add_event(ENCODER_RIGHT);
        break;

        case ENC_CLICK:
            add_event(ENCODER_PRESS);
        break;

        case ENC_LONG:
            add_event(ENCODER_LONG);
        break;

        case ENC_DOUBLE:
            add_event(ENCODER_DOUBLE);
        break;

        default:
        break;
    }
}

/**********************************************************
*********************  DISPLAY   **************************
**********************************************************/

// Muestra el brillo actual en el display
void brillo_init() {
	dispArrShowNum(dispArrGetBright());
}

// Decrementa el brillo del display
void dec_bright() {
	dispArrSetBright(dispArrGetBright()-1);
	dispArrShowNum(dispArrGetBright());
}

// Incrementa el brillo del display
void inc_bright() {
	dispArrSetBright(dispArrGetBright()+1);	
	dispArrShowNum(dispArrGetBright());
}

// La pantalla de error es la misma para todos los estados
void errorScreen() {
	dispArrSlideLoop(ERROR_MSG);
}


void update_display(int* arr, int counter, bool password) {

	uint8_t dispIndex = min(counter, DISP_COUNT-1);

	uint8_t dispOffset = counter < DISP_COUNT ? counter : counter - DISP_COUNT*(counter/DISP_COUNT);

	if (password) {

		char strArr[DISP_COUNT];

		for (int i = 0; i < DISP_COUNT; i++) {
			strArr[i] = '-';
		}

		strArr[dispIndex] = arr[counter];

		dispArrShowNumArr(strArr);
		
	}
	else {
		dispArrShowNumArr(arr + dispOffset);
	}
	
	dispArrSelect(dispIndex);

}


void updateMenuDis(char* word) {
	dispArrSlideLoop(word);
}

/**********************************************************
*********************  VARIOUS   **************************
**********************************************************/
void doNothing() {
    return;
}