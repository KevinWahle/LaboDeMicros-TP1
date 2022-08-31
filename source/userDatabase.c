/******************************************************************************
  @file     userDataBase.c
  @brief    Base de datos de usuarios
  @author   Fran Basili
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include "const.h"
#include "event_queue/event_queue.h"
#include "userDatabase.h"

/*******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
static uint16_t indice;
static uint16_t usercount;
static user database[MAXUSER];


typedef struct{
    uint8_t id[IDSIZE];
    uint8_t password[PASSMAX];
} user;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t searchUser(uint8_t * id);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void list_init(){
    indice=0;
    updateMenuDis(database[indice].id);
}

void up_menu_del(){
    if(indice>0){
        indice--;
        updateMenuDis(database[indice].id);    
    }
}

void down_menu_del(){
    if(indice<MAXUSER){
        indice++;
        updateMenuDis(database[indice].id);
    }
} 

void del_user(){
    if(usercount>0){
        copyUser();
        usercount--;
        add_event(BACK);
    }
}

void copyUser(){
    for(int i=0; i<IDSIZE; i++){
        ((database[indice]).id)[i]=((database[usercount]).id)[i];
    }

    for(int i=0; i<PASSMAX; i++){
        ((database[indice]).password)[i]=((database[usercount]).password)[i];
    }
}

bool internal_check_id(const uint8_t * id){
    bool found = false;
    bool nullID = false;
    for (uint8_t i = 0; i < IDSIZE && !NULLCHAR; i++){
        if(id[i] == NULLCHAR)
            nullID = true;
    }
    
    if(!nullID){
        for (uint8_t i = 0; i < usercount && !found; i++){
            found = true;
            for (uint8_t j = 0; j < IDSIZE; j++){
                if(database[i].id[j] != id[j])
                    found = false;
            }
        }
    }
    return found;
}

bool internal_check_pass(uint8_t * id, uint8_t * password){
    
    // Verifico que la password 
    for (uint8_t i = 0; i < PASSMIN; i++){
        if(password[i] == NULLCHAR)
            return false;
    }

    uint8_t indice = searchUser(id);    //TODO: por que falla?
    if (indice==MAXUSER)
        return false;
    return passwordEquals(password, indice);
}

uint8_t searchUser(uint8_t * id){
    uint8_t equal=true;

    for (uint8_t user=0; user<usercount; user++){
        equal=true;

        for (uint8_t digit=0; digit<IDSIZE && equal; digit++){
            if(id[digit] != ((database[user]).id)[digit]){
                equal=false;
            }
            else if(digit==IDSIZE-1)
                return user;
        }
    }
    return MAXUSER;
}

uint8_t passwordEquals(uint8_t* password,uint8_t indice){
    for(uint8_t digit=0; digit<PASSMAX; digit++){
        if(password[digit] != ((database[indice]).id)[digit])
            return false;
    }
    return true; 
}
   

void internal_used_id(uint8_t * id){
    if(searchUser(id)!=MAXUSER) // Verifico si el usuario existe
        return true;

    return false;
}

bool internal_save_pass(uint8_t * id, uint8_t * pass){
    uint8_t indice=searchUser(id);
    if (indice>=MAXUSER){
        return false;
    }

    for (uint8_t i = 0; i < PASSMAX; i++){
        (database[indice]).password[i] = pass[i];
    }
    return true;
}
   
bool internal_add_user(uint8_t * id, uint8_t * pass){
    if(usercount >= MAXUSER)
        return false;

    else{
        for (uint8_t i = 0; i < IDSIZE; i++){
            (database[indice]).id[i] = id[i];
        }

        for (uint8_t i = 0; i < PASSMAX; i++){
            (database[indice]).password[i] = pass[i];
        }
        usercount++;
    }
}

/*******************************************************************************
 ******************************************************************************/
