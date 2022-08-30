/***************************************************************************//**
  @file     App.c
  @brief    TP!: Access Control
  @author   Sergio Peralta
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "const.h"
#include "FSM_table.h"
#include "FSM_routines.h"
#include "event_queue/event_queue.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        	GLOBAL VARIABLES
 *******************************************************************************
 ******************************************************************************/
static STATE* p_tabla_estado_actual;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	p_tabla_estado_actual = ID_state;
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	event_t evento = get_next_event();  // Tomo un nuevo evento de la cola de eventos.
	
	#ifdef DEBUG
		if (evento != NULL_EVENT){
		printf("El nuevo evento es: %d \n", evento);
		}
	#endif

	p_tabla_estado_actual = fsm_interprete(p_tabla_estado_actual, evento);  // Actualizo el estado
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

STATE *fsm_interprete(STATE * p_tabla_estado_actual, event_t evento_actual)
{
    #ifdef DEBUG
        printf("Evento: %d. \n", evento_actual); // Para debuggear
    #endif  //DEBUG
    
    while ((p_tabla_estado_actual -> evento) != evento_actual && (p_tabla_estado_actual -> evento) !=NULL_EVENT){
        ++p_tabla_estado_actual;
    } 

    (p_tabla_estado_actual -> p_rut_accion) (); // Ejecuta Rutina de accion correspondiente
    p_tabla_estado_actual = (p_tabla_estado_actual -> proximo_estado); // Encuentro próximo estado
    return (p_tabla_estado_actual);    
}

/*******************************************************************************
 ******************************************************************************/
