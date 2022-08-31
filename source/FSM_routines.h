/*******************************************************************************
  @file     +FSM_routines.h+
  @brief    +Funciones que conforman las rutas de acción entre los diferentes
            estados de la FSM.+
  @author   +Grupo 3+
 ******************************************************************************/

#ifndef _FSM_ROUTINES_H_
#define _FSM_ROUTINES_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// funciones de serman
void brillo_init(); // Muestra el brillo actual en el display               //SERMAN
void dec_bright();  // decrementa la var brillo y refresca                  //SERMAN
void inc_bright();  // incrementa la var brillo y refresca                  //SERMAN

void errorScreen(); // La pantalla de error es la misma para todos los estados //SERMAN

void id_init();     // reset del arreglo id y su puntero
void previous_id(); // movemos puntero para atras
void upper_id();     
void next_id();
void check_id();    // existe el id?                                        //SCHEMBE

void pass_init();   // reset del arreglo pass y su puntero
void previous_pass();
void upper_pass();
void next_pass();   
void check_pass();  // la pass es la indicada                               //SCHEMBE

void init_admin_menu(); // reset el option selector
void up_menu_Admin();
void down_menu_Admin();
void click_menu_Admin();    // genera el evento correspondiente a la opcion seleccionada

void init_menu();   // reset el option selector
void down_menu();
void up_menu();
void click_menu();

void used_id();     // el id a agregar ya existe? Genera ID_OK o ID_WRONG   //SCHEMBE
void save_pass();   // dsepues de guardar se genera el evento BACK          //SCHEMBE
void add_user();    // addUser genera el evento back al terminar            //SCHEMBE

void list_init();   // inicia la lista de usuarios para seleccionar cual borrar  
void up_menu_del();
void down_menu_del(); 
void del_user(); //SCHEMBE

// Creación, paseo y seleccion de los menus: BASILI
// Carga de ID y contraseña: BASILI
// Base de datos, Borrado y agregado de usuarios: Schembe


/**
 * @brief Llama a la función que se encarga de mostrar el menu, 
 *        indicandole las opciones a mostrar y la opcion a resaltar
*/
void my_menu(void);


/**
 * @brief Llama a la función que se encarga de mostrar el menu que se le pase, 
 *        indicandole las opciones a mostrar y la opcion a resaltar
 * @param menu menu a mostrar en pantalla
 * @param menu_size tamaño del menu
*/
void up_menu(MENU_ITEM* menu, int menu_size);


/**
 * @brief Llama a la función que se encarga de mostrar el menu que se le pase, 
 *        indicandole las opciones a mostrar y la opcion a resaltar
 * @param menu: menú a mostrar en pantalla.
 * @param menu_size: tamaño del menú.
*/
void down_menu(MENU_ITEM* menu, int menu_size);


/**
 * @brief Añade la opcion seleccionada en el menú general a la cola de eventos.
*/
void click_menu(void);


/**
 * @brief Llama a la función que se encarga de mostrar el menu de pausa, 
 *        indicandole las opciones a mostrar y la opción a resaltar.
*/
void pause_game(void);


/**
 * @brief Reanuda la partida.
*/
void resume_game(void);


/**
 * @brief Inicializa y/o resetea los puntos y las variables propias del juego.
*/
void start_game(void);


/**
 * @brief LLama a la función que se encarga de mostrar el score, indicandole el 
 *        puntaje final y la cantidad de aliens asesinados.
*/
void show_game_score();


/**
 * @brief LLama a la función que se encarga de mostrar el tablero de los top score, 
 *        por constantes del programa se encarga de determinar el tamaño del mismo.
*/
void show_global_score(void);


/**
 * @brief LLama a la función que se encarga de destruir los recursos del front.
*/
void quit_game(void);


/**
 * @brief No hace nada. Su creación fue necesaria a causa de la arquitectura 
 *        de la máquina de estados. 
*/
void doNothing(void);


/**
 * @brief Añade la opcion seleccionada en el menú de pausa a la cola de eventos.
*/
void click_menu_pause(void);


/**
 * @brief Llama a la función encargada seleccionar la opcion de arriba del menu principal.
*/
void up_menu_main(void);


/**
 * @brief Llama a la función encargada seleccionar la opcion de abajo del menu principal.
*/
void down_menu_main(void);


/**
 * @brief Llama a la función encargada seleccionar la opcion de arriba del menu de pausa.
*/
void up_menu_pause(void);


/**
 * @brief Llama a la función encargada seleccionar la opcion de abajo del menu de pausa.
*/
void down_menu_pause(void);


/**
 * @brief pasa a la letra siguiente del nombre para poder modificarla.
*/
void next_letter(void);

/**
 * @brief vuelve a la letra anterior del nombre para poder modificarla.
*/
void previous_letter(void);

/**
 * @brief pasa a la siguiente letra del abcedario.
*/
void upper_letter(void);

/**
 * @brief pasa a la letra anterior del abcedario.
*/
void lower_letter(void);

/**
 * @brief Guarda en un .txt el puntaje y el nombre correspondiente.
*/
void save_score(void);


/**
 * @brief inicializa las variables para poder cargar el nombre.
*/
void saving_init(void);

/**
 * @brief Actualiza el timer y lo que se muestra en pantalla.
*/
void refresh (void);

/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_ROUTINES_H_
