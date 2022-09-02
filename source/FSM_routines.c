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
#include "encoder_hal.h"
#include "const.h"
#include "userDatabase.h"
#include "timer/timer.h"
#include <stdio.h>
#include "display/dispArr.h"
#include "LEDMux/LEDMux.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ID_SHOW_TIME	5000

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
static uint8_t actual_id[IDSIZE]={NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR};
static uint8_t actual_pass[PASSMAX]={NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR,NULLCHAR};
static uint8_t digitCounter;
static tim_id_t idTimer;

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
void setIDTimer_cb();

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**********************************************************
************************  ID  ***************************
**********************************************************/
void id_init(){
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
    inactivityTimer();
}

void upper_id(){
    if (actual_id[digitCounter]<9)
    {
        actual_id[digitCounter]++;
    }

    else if(actual_id[digitCounter]==9)
        actual_id[digitCounter]=0;

    else if(actual_id[digitCounter]==NULLCHAR)
        actual_id[digitCounter]=0;
    
    update_display(actual_id, digitCounter, 0);
    inactivityTimer();
}

void next_id(){
    if(digitCounter < IDSIZE -1 && actual_id[digitCounter]!=NULLCHAR){
        digitCounter++;
        inactivityTimer();
    }

    else if(actual_id[digitCounter]==NULLCHAR){
        inactivityTimer();
    }

    else if(digitCounter >= IDSIZE -1){
        add_event(ID_READY);
    }

    update_display(actual_id, digitCounter, 0);
}

void check_id(void){

    if(internal_check_id(actual_id))
        add_event(ID_OK);
    else
        add_event(WRONG_ID);
} 

void used_id(){
    if(internal_used_id(actual_id) || !avaliableUsers())
        add_event(WRONG_ID);
    else
        add_event(ID_OK);
}  

void setUpIDTimer(){
    idTimer=timerGetId();
}

void setIDTimer(){
    timerStart(idTimer, TIMER_MS2TICKS(ID_SHOW_TIME), TIM_MODE_SINGLESHOT, setIDTimer_cb);
    char char_id[IDSIZE+1];

    for(uint8_t digit=0; digit<IDSIZE; digit++){
    	char_id[digit]= (char)actual_id[digit]+'0';
	}

    char_id[IDSIZE] = '\0';

    dispArrSlideOnce(char_id);
}

void setIDTimer_cb(){
    add_event(TIMEOUT);
}
/**********************************************************
************************  PASSWORD  ***********************
**********************************************************/

void pass_init(){
    for(int i=0; i<PASSMAX; i++)
    {
        actual_pass[i]=NULLCHAR;
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
    inactivityTimer();
}

void upper_pass(){
    if (actual_pass[digitCounter]<9)
        actual_pass[digitCounter]++;

    else if(actual_pass[digitCounter]==NULLCHAR)
        actual_pass[digitCounter]=0;

    else if(actual_pass[digitCounter]==9)
        actual_pass[digitCounter]=NULLCHAR;
    
    update_display(actual_pass, digitCounter, 1);
    inactivityTimer();
}

void next_pass(){
    if(digitCounter < PASSMAX-1){
        digitCounter++;
    }

    update_display(actual_pass, digitCounter, 1);
    inactivityTimer();
}

void check_pass(){
    if(internal_check_pass(actual_id, actual_pass))
        if(isAdmin(actual_id))
        	add_event(ADMIN_USER);
		else
			add_event(NORMAL_USER);
    else
        add_event(WRONG_PASS);
}  

void save_pass(){
    if(internal_save_pass(actual_id, actual_pass))
        add_event(BACK);
    else
        add_event(RESET);
}

void add_user(){
    if(internal_add_user(actual_id, actual_pass))
        add_event(BACK);
    else
        add_event(BACK);
} 

void verifyPass(){
    if(internal_verifyPass(actual_id))
        add_event(PASS_READY);
    else
        add_event(WRONG_PASS);
}
/**********************************************************
************************  MENU  ***************************
**********************************************************/
void admin_allow_access(){
    init_admin_menu();
    LEDMuxSetForTime(2, 5000);
}

void init_admin_menu(){
    actual_option=0;
    updateMenuDis(admin_menu[actual_option].option);
}

void up_menu_Admin(){
    if (actual_option>0){
        actual_option--;
        updateMenuDis(admin_menu[actual_option].option);
    }
    inactivityTimer();
}

void down_menu_Admin(){
    if (actual_option<ADMIN_MENU_LEN-1){
        actual_option++;
        updateMenuDis(admin_menu[actual_option].option);
    }
    inactivityTimer();
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

void user_allow_access(){
    init_menu();
    LEDMuxSetForTime(2, 5000);
}

void init_menu(){
    actual_option=0;
    updateMenuDis(user_menu[actual_option].option);
    inactivityTimer();
}

void down_menu(){
    if (actual_option<USER_MENU_LEN-1){
        actual_option++;
        updateMenuDis(user_menu[actual_option].option);
    }
    inactivityTimer();
}

void up_menu(){
    if (actual_option>0){
        actual_option--;
        updateMenuDis(user_menu[actual_option].option);
    }
    inactivityTimer();
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

void del_user(){
    internal_del_user(actual_id);
}

/**********************************************************
********************  CALLBACKS  *************************
**********************************************************/

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

void IDcardCb (bool state, const char* mydata){
    if(state){
        for(uint8_t digit=0; digit<IDSIZE; digit++)
        {
            actual_id[digit]= (uint8_t)(mydata[digit]-'0');
        }
        add_event(ID_READY);
    }
    else{
        add_event(ERROR_CARD);
    }
}

void inactivityTimer(){
    timerStart(idTimer, TIMER_MS2TICKS(30000), TIM_MODE_SINGLESHOT, setIDTimer_cb);
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


void update_display(uint8_t* arr, uint8_t counter, bool password){

	uint8_t dispIndex = counter < (DISP_COUNT-1) ? counter : (DISP_COUNT-1);
	uint8_t dispOffset = counter < DISP_COUNT ? 0 : counter - DISP_COUNT*(counter/DISP_COUNT) + 1;

	if (password) {

		char strArr[DISP_COUNT];

		for (int i = 0; i < DISP_COUNT; i++) {
			strArr[i] = '-';
		}

		strArr[dispIndex] = (char) arr[counter]+'0';

		dispArrShow(strArr);
		
	}
	else {
		dispArrShowNumArr(arr + dispOffset);
	}
	
	dispArrSelect(dispIndex);

}

void updateMenuDis(char* word) {
	dispArrSlideLoop(word);
}

void updateListDis(uint8_t* id){
	char word[IDSIZE+1];
	for(uint8_t i=0; i<IDSIZE; i++)
	{
		word[i]= id[i] + '0';
	}
    word[IDSIZE]='\0';
	dispArrSlideLoop(word);
}

/**********************************************************
*********************  VARIOUS   **************************
**********************************************************/
void doNothing() {
    return;
}

