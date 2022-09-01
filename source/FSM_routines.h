/******************************************************************************
  @file     userDataBase.c
  @brief    Base de datos de usuarios
  @author   Fran Basili
 ******************************************************************************/

#ifndef _FSM_ROUTINES_H_
#define _FSM_ROUTINES_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "encoder_hal.h"
#include "stdint.h"

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Muestra el brillo actual en el display
*/
void brillo_init();   

/**
 * @brief decrementa la var brillo y refresca
*/
void dec_bright();                  

/**
 * @brief incrementa la var brillo y refresca
*/
void inc_bright();                 

/**
 * @brief La pantalla de error es la misma para todos los estados
*/
void errorScreen(); 

/**
 * @brief reset del arreglo id y su puntero
*/
void id_init();

/**
 * @brief movemos puntero para atras
*/
void previous_id();

/**
 * @brief Incrementa el digito actual del ID
*/
void upper_id();     

/**
 * @brief Pasa a modificar el sgte digito
*/
void next_id();

/**
 * @brief Checkea si el ID existe
*/
void check_id();

/**
 * @brief Reset del arreglo password y su puntero
*/
void pass_init();   

/**
 * @brief movemos puntero para atras
*/
void previous_pass();

/**
 * @brief Incrementa el digito actual de la pass
*/
void upper_pass();

/**
 * @brief Pasa a modificar el sgte digito
*/
void next_pass();   

/**
 * @brief Verifica que la password se corresponda 
*/
void check_pass();

/**
 * @brief Reinicia el selector 
*/
void init_admin_menu(); 

/**
 * @brief mueve el selector del menu hacia arriba y actualiza display 
*/
void up_menu_Admin();

/**
 * @brief mueve el selector del menu hacia abajo y actualiza display 
*/
void down_menu_Admin();

/**
 * @brief genera el evento correspondiente a la opcion seleccionada
*/
void click_menu_Admin();

/**
 * @brief Reinicia el selector 
*/
void init_menu();

/**
 * @brief mueve el selector del menu hacia arriba y actualiza display 
*/
void up_menu();

/**
 * @brief mueve el selector del menu hacia abajo y actualiza display 
*/
void down_menu();

/**
 * @brief genera el evento correspondiente a la opcion seleccionada
*/
void click_menu();

/**
 * @brief Genera un ID_OK si el id no existe, un ID_WRONG sino
*/
void used_id();     

/**
 * @brief Guarda la contraseña cuando queres cambiarla
*/
void save_pass();   

/**
 * @brief Guarda el usuario en la base de datos
 *        Genera el evento BACK una vez terminado el proceso
*/
void add_user();

/**
 * @brief Arranca la lista de usuarios para saber cual borrar
*/
void list_init();

/**
 * @brief subimos en el menú de borrado
*/
void up_menu_del();

/**
 * @brief bajamos en el menú de borrado
*/
void down_menu_del(); 

/**
 * @brief borramos el usuario
*/
void del_user();

/**
 * @brief Callback del Enconder
*/
void encoderCallback(ENC_STATE state);

/**
 * @brief Callback del Enconder
*/
void doNothing();

/**
 * @brief Inicializa el timer
*/
void setIDTimer();

/**
 * @brief Pide id de timer
*/
void setUpIDTimer();

/**
 * @brief Callback Tarjeta Magnética
 */
void IDcardCb (bool state, const char* mydata);

/**
 * @brief enciende el timer de inactividad
 */
void inactivityTimer();

/**
 * @brief Muestra en el display ints
 */
void updateListDis(uint8_t* id);

/**
 * @brief Prende led y carga menu para admin
 */
void admin_allow_access();

/**
 * @brief Prende led y carga menu para no admin
 */
void user_allow_access();

/**
 * @brief Verifica que la password sea válida
 */
void verifyPass();
/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_ROUTINES_H_
