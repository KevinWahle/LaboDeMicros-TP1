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
#include "const.h"
#include <stdio.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

bool running = 1;

MENU_ITEM main_menu[] = {  
                            {.option = "Jugar", .essential = true, .ID = PLAY_ID},
                            {.option = "Puntajes", .essential = true, .ID = SCORE_ID},
                            {.option = "Instrucciones", .essential = false, .ID = INSTUCTION_ID},
                            {.option = "Salir", .essential = true, .ID = EXIT_ID},
                        };

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int actual_option = 0;                            // Variable que marca la opcion del menú seleccionada.   
static char actual_name[NAME_SIZE+1];
static SCORE leadboard[LEADERBOARD_SIZE+1];                // Creo matriz con el leaderboard  
static int letter_counter=0;                             // Variable que da el indice de la letra a cargar.
static int letter;                                       // Variable que retiene la letra que se quiere cargar.   

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**********************************************************
************************  MENU  ***************************
**********************************************************/

void my_menu(){
    actual_option=0;
    show_menu (main_menu, sizeof(main_menu)/sizeof(MENU_ITEM), actual_option);  //Actualizo el menu, resaltando la opcion actualizada.  
    #ifdef DEBUG
        printf("Muestro el menú principal. \n");
    #endif

}

void up_menu(MENU_ITEM* menu, int menu_size){ 
    
    #ifdef ONLY_ESSENTIAL                                                    
            #ifdef DEBUG
                printf("Estoy en modo ONLY_ESSENTIAL \n");
            #endif

            do{                                                                
                
                if(actual_option>0){                            // Si el front solo permite mostrar las opciones esenciales:
                    actual_option--;                                                        //subimos en el menú hasta la siguiente opcion esencial siempre
                }                                                                           //y cuando haya una arriba.
            } while ((menu[actual_option]).essential==false && menu_size/sizeof(MENU_ITEM) > actual_option); 
    
    #else                                                                   // Si el front permite mostrar las opciones no esenciales:
        if(actual_option > 0){        
            actual_option--;                                                //subimos en el menú hasta la siguiente opcion
        }

    #endif

    #ifdef DEBUG
    printf("La nueva opción actual es: %d \n", main_menu[actual_option].ID);
    #endif 

    show_menu (menu, menu_size/sizeof(MENU_ITEM), actual_option);          // Actualizamos el front. 

    #ifdef DEBUG
    printf("Se actualizó el menú \n");
    #endif 
}

void down_menu(MENU_ITEM* menu, int menu_size){
    
    #ifdef ONLY_ESSENTIAL   
        #ifdef DEBUG
                printf("Estoy en modo ONLY_ESSENTIAL \n");
        #endif         
        do{
           if(menu_size/sizeof(MENU_ITEM)-1 > actual_option) {                          // Si el front solo permite mostrar las opciones esenciales:
                actual_option++;                                                        //bajamos en el menú hasta la siguiente opción esencial siempre
           }                                                                            //y cuando haya una abajo.
        } while ((menu[actual_option]).essential==false && actual_option>0);
    
    #else                                                                               // Si el front permite mostrar las opciones no esenciales:
        
        if(menu_size/sizeof(MENU_ITEM)-1 > actual_option) {
            actual_option++;                                                            //bajamos en el menú hasta la siguiente opcion
        }
        
    #endif
    

    #ifdef DEBUG
    printf("La nueva opción actual es: %d \n", main_menu[actual_option].ID);
    #endif 
    
    show_menu (menu, menu_size/sizeof(MENU_ITEM), actual_option);                       // Actualizamos el front. 

    #ifdef DEBUG
    printf("Se actualizó el menú \n");
    #endif 
}

void up_menu_main()
{
    up_menu(main_menu, sizeof(main_menu));              // Subo en el menu principal
}

void down_menu_main()
{
    down_menu(main_menu, sizeof(main_menu));            // Bajo en el menu principal
}

void up_menu_pause()
{
    up_menu(pause_menu, sizeof(pause_menu));             // Subo en el menu de pausa
}

void down_menu_pause()
{
    down_menu(pause_menu, sizeof(pause_menu));           // Bajo en el menu de pausa
}


void click_menu()
{     
    switch ((main_menu[actual_option]).ID)
    {
        case PLAY_ID:
            add_event(PLAY_EVENT);        // Añadimos a  la cola de eventos
        break;

        case SCORE_ID:
            add_event(SCORE_EVENT);       // Añadimos a  la cola de eventos
        break;

        case INSTUCTION_ID:
            add_event(INSTRUCTION_EVENT);     // Añadimos a  la cola de eventos
        break;

        case EXIT_ID:
            add_event(EXIT_EVENT);        // Añadimos a  la cola de eventos
        break;        
    }

    #ifdef DEBUG
        printf("Se agregó a la cola de eventos el ID: %d \n", main_menu[actual_option].ID);
    #endif      
    actual_option=0;
}

void click_menu_pause()
{      

    switch (pause_menu[actual_option].ID)
    {
        case PLAY_ID:
            add_event(PLAY_EVENT);            // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM.
        break;

        case RESUME_ID:
            add_event(RESUME_EVENT);          // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM. 
        break;
        
        case BACK_ID:
            add_event(BACK_EVENT);            // Añadimos a  la cola de eventos
                                                    //el evento para redireccionar la FSM.
        break;
    }

    actual_option=0;

    #ifdef DEBUG
        printf("Se agregó a la cola de eventos: %d \n", main_menu[actual_option].ID);
        
    #endif 
}


void next_letter()
{
    if (letter_counter < IDSIZE-1)                         // Si me quedan letras por guardar:
    {
    letter_counter++;                              // Paso a la siguiente letra.                                           
    letter=actual_name[letter_counter];            // Cargo la siguiente letra de la nueva ubicación.
    
    if(letter==9)    
    {
        actual_name[letter_counter]='A';
    } 

    #ifdef DEBUG
        printf("Se confirmo la letra: %c. El arreglo quedó %s.\n", actual_name[letter_counter], actual_name);
    #endif   
    
    }
}

void next_digit(){
    if (digit_counter < IDSIZE-1){
        digit_counter++;
        digit=actual_word[]
    }
}

void previous_letter()
{
    if(letter_counter>0)                                // Si no estoy en la primer letra:
    {
        letter_counter--;                               //Retrocedo una letra.
        #ifdef DEBUG
            printf("Se retrocedió una letra \n");
        #endif
        
        letter=actual_name[letter_counter];             // Cargo la anterior letra de la nueva ubicación.

    
    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.

    }   
}

void upper_letter()
{
    if (letter=='Z'){                           // Si tengo cargada una 'Z'                        
        letter= '_';                            //cargo el caracter ' '.
    }  
    else if (letter=='_'){                      // Si tengo cargada un ' '                        
        letter= 'A';                            //cargo el caracter 'A'.
    }    

    else{
        letter++;                               // Si es una letra cualquiera paso a la siguiente del
                                                //abcedario.
    }
    #ifdef DEBUG
        printf("Se pasó a la letra %c \n", letter);
    #endif
    actual_name[letter_counter]=letter;

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.
}

void lower_letter()
{
    if (letter=='A'){                           // Si tengo cargada una 'A'                        
        letter= '_';                            //cargo el caracter ' '.
    }  
    else if (letter=='_'){                      // Si tengo cargada un ' '                        
        letter= 'Z';                            //cargo el caracter 'Z'.
    }    
    else 
    {                                           // Si es una letra cualquiera paso a la anterior del
        letter--;                               //abcedario.
    }
    #ifdef DEBUG
        printf("Se pasó a la letra %c\n", letter);
    #endif
    actual_name[letter_counter]=letter;

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points()); // Muestra la pantalla de carga de puntaje.
}

void save_score(){

    #ifdef DEBUG
    printf("El nombre a guardar es: %s\n", actual_name);
    #endif

    SCORE* p_leadboard=leadboard;                       // Coloco un puntero a su primer elemento
    int not_null_char=0;                                // Creo una variable que cuente los ' '.
    for(int i=0; i<NAME_SIZE; i++)                      // Para cada caracter no terminador de
    {                                                   //actual_name:
        if (actual_name[i] != '_'){                     // Reviso si NO es ' ' y si es así              
            not_null_char++;}                           // incremento la cantidad de posiciones
                                                        // que tinen valor.
        else{
            actual_name [i] = ' ';       
        }
    }                                                   

    if (not_null_char>0)                                // Si afirmativamente hay algun caracter guardado:
    {

        put_score (actual_name, get_points(), LEADERBOARD_SIZE, p_leadboard);   // Ejecuto la función que guarda el 
                                                                                //puntaje ordenado en el archivo.

        #ifdef DEBUG
        printf("Se guadro el nuevo score\n");
        #endif
    }

    my_menu();                                          // Precargo el menú antes de salir.
}

void saving_init()
{
    actual_name[0]='A';

    for(int i=1; i<NAME_SIZE; i++)
    {
        actual_name[i]= '_';        // Inicio el arreglo con espacios.
    }
    actual_name[NAME_SIZE]=0;       // Cargo el terminador.
    letter_counter=0;               // Apunto al primer elemento del arreglo nombre
    letter= actual_name[letter_counter];

    #ifdef DEBUG
        printf("Se inicializó la carga \n");
    #endif

    score_name_front(actual_name, NAME_SIZE, letter_counter, get_points());    
}

/**********************************************************
*********************  VARIOUS   **************************
**********************************************************/

void refresh(void){
    speed_update(SPEED_LAPSE);                                  // Actualizo la velocidad con la que se mueven los invaders.
    redraw(get_points(),get_lives(),get_level());               // Redibujo la pantalla.
    if(checkWin()){                                             // Verifico si se pasó de nivel o no.
        add_event(NEXT_LEVEL_EV);
    }
}

void doNothing() {
    return;
}