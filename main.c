#include "juegoPrincipal.h"
#include "listaSimple.h"

int main()
{
    char decisionJugador[MAX_NOMBRE];
    tLista listaJugadores;

    menu(decisionJugador);
    if(strcmp(decisionJugador, "A")== 0){

        if(jugar(&listaJugadores) == HAY_ERROR){
            printf("Ocurrio un error al ingresar los jugadores \n");
            return HAY_ERROR;
        }
    }

    return 0;
}
