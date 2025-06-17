#include "juegoPrincipal.h"
#include "listaSimple.h"

int main()
{
    char decisionJugador[MAX_NOMBRE];
    tLista listaJugadores;
    char nombreArch[20] = "config.txt";
    menu(decisionJugador);

    crearArchivoConfig(nombreArch, 2);
    if(strcmp(decisionJugador, "A")== 0){
        if(jugar(&listaJugadores,nombreArch) == ERROR){
            printf("Ocurrio un error al ingresar los jugadores \n");
            return HAY_ERROR;
        }
    }

    return 0;
}
