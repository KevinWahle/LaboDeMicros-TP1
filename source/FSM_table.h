/*******************************************************************************
  @file     +FSM_table.h+
  @brief    +Tabla de estados de la maquina de estados+
  @author   +Grupo 3+
 ******************************************************************************/

#ifndef FSM_TABLE_H
#define FSM_TABLE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "FSM_routines.h"
#include "event_queue/event_queue.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 *******************************************************************************/
typedef struct state_diagram_edge STATE;
struct state_diagram_edge{
    event_t evento;
	//Evento que activa la rutina de acción

    STATE *proximo_estado;
    // Puntero que apunta al estado al que hay que ir en caso
    //que se cumpla el evento correspondiente. 

    void (*p_rut_accion)(void);
    // Función a realizar durante la transición entre un estado y otro.
};

STATE *fsm_interprete(STATE * p_tabla_estado_actual, event_t evento_actual);

extern STATE ID_state[];
extern STATE brillo_state[];
extern STATE IDError_state[];
extern STATE Pass_state[];
extern STATE PassError_state[];
extern STATE changePass_state[];
extern STATE addID_state[];
extern STATE addError_state[];
extern STATE addPass_state[];
extern STATE delUser_state[];

STATE ID_state[] = {
    {ENCODER_LONG, brillo_state, brillo_init},          
    {ENCODER_LEFT, ID_state, fill},  // Revisar previous_letter
    {ENCODER_RIGHT, ID_state, fill}, // Revisar upper_letter
    {ENCODER_PRESS, ID_state, fill}, // Revisar next_letter
    {ID_READY, ID_state, check_id},
    {WRONG_ID, IDError_state, errorScreen},     
    {ID_OK, Pass_state, pass_init},
    {NULL_EVENT, ID_state, doNothing}
};

STATE brillo_state[] = {
    {ENCODER_LEFT, brillo_state, dec_bright},  
    {ENCODER_RIGHT, brillo_state, inc_bright},
    {ENCODER_PRESS, ID_state, id_init}, // Revisar next_letter 
    {NULL_EVENT, brillo_state, doNothing}
};

STATE IDError_state[] = {
    {TIMEOUT, IDError_state, id_init},
    {NULL_EVENT, IDError_state, doNothing}
};

STATE Pass_state[] = {
    {ENCODER_LEFT, Pass_state, fill},  // Revisar previous_letter
    {ENCODER_RIGHT, Pass_state, fill}, // Revisar upper_letter
    {ENCODER_PRESS, Pass_state, fill}, // Revisar next_letter
    {ENCODER_LONG, ID_state, id_init}, 
    {PASS_READY, Pass_state, check_pass},
    {PASS_OK, Menu_state, my_menu},
    {NULL_EVENT, Pass_state, doNothing}
};

STATE PassError_state[] = {
    {TIMEOUT, Pass_state, pass_init},
    {NULL_EVENT, PassError_state, doNothing}
};

STATE Menu_state[] = {
    {ENCODER_LEFT, Menu_state, up_menu_main},       //REVISAR: el callback
    {ENCODER_RIGHT, Menu_state, down_menu_main},    //REVISAR: el callback
    {ENCODER_PRESS, Menu_state, click_menu},        //REVISAR: el callback
    {CHNG_PASS, changePass_state, pass_init},
    {ADD_USER, addID_state, id_init},
    {DEL_USER, delUser_state, list_init},
    {BACK, ID_state, id_init},
    {NULL_EVENT, Menu_state, doNothing}
};

STATE changePass_state[] = {
    {ENCODER_LEFT, changePass_state, fill},  // Revisar previous_letter
    {ENCODER_RIGHT, changePass_state, fill}, // Revisar upper_letter
    {ENCODER_PRESS, changePass_state, fill}, // Revisar next_letter
    {ENCODER_LONG, Menu_state, my_menu},
    {PASS_READY, Pass_state, save_pass},
    {BACK, Menu_state, my_menu},
    {NULL_EVENT, changePass_state, doNothing}
};

STATE addID_state[] = {
    {ENCODER_LEFT, addID_state, fill},  // Revisar previous_letter
    {ENCODER_RIGHT, addID_state, fill}, // Revisar upper_letter
    {ENCODER_PRESS, addID_state, fill}, // Revisar next_letter
    {ENCODER_LONG, Menu_state, my_menu},
    {ID_READY, addID_state, used_id},   // used_id genera ID_OK o WRONG_ID 
    {WRONG_ID, addError_state, errorScreen},     
    {ID_OK, addPass_state, pass_init},
    {NULL_EVENT, addID_state, doNothing}
};

STATE addError_state[] = {
    {TIMEOUT, addID_state, id_init},
    {NULL_EVENT, addError_state, doNothing}
};

STATE addPass_state[] = {
    {ENCODER_LEFT, addPass_state, fill},  // Revisar previous_letter
    {ENCODER_RIGHT, addPass_state, fill}, // Revisar upper_letter
    {ENCODER_PRESS, addPass_state, fill}, // Revisar next_letter
    {ENCODER_LONG, Menu_state, my_menu},
    {PASS_READY, Pass_state, add_user},     // addUser genera el evento back al terminar     
    {BACK, Menu_state, my_menu},
    {NULL_EVENT, addPass_state, doNothing}
};

STATE delUser_state[] = {
    {ENCODER_LEFT, delUser_state, up_menu_main},    //REVISAR: el callback
    {ENCODER_RIGHT, delUser_state, down_menu_main}, //REVISAR: el callback
    {ENCODER_PRESS, delUser_state, del_user},       //del user genera back 
    {BACK, Menu_state, my_menu},
    {NULL_EVENT, DelUser_state, doNothing}
};

/*******************************************************************************
 ******************************************************************************/

#endif // FSM_table_H