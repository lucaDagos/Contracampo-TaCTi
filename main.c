#include "juegoPrincipal.h"
#include "informePartidas.h"
#include "listaSimple.h"

int main(){

    char decisionJugador[MAX_NOMBRE];
    tLista listaJugadores;
    tLista listaPartidas;
    tLista listaRanking;
    tConfiguracion config;
    char nombreArch[20] = "config.txt";

    listaCrear(&listaPartidas);
    listaCrear(&listaRanking);

    crearArchivoConfig(nombreArch, 2);

    obtenerDatosArchivoConfiguracion("configuracion.txt", &config);


    menu(decisionJugador);

    if(strcmp(decisionJugador, "A") == 0){

        if(jugar(&listaJugadores, &listaPartidas, nombreArch) == ERROR){
            printf("Ocurrio un error al ingresar los jugadores.\n");
            return HAY_ERROR;
        }

        //Ordenar lista de jugadores por puntaje total (mayor a menor)
        ordenarLista(&listaJugadores, compararPuntajeTotal);

        crearInformePartidas(&listaJugadores, &listaPartidas, config.CantPartidas, compararIgualdadPuntajeTotal);

        printf("\nEnviando datos a la API...");
        enviarDatosListaAPI(&listaJugadores, &config, enviarDatosJSON);

        listaVaciar(&listaJugadores);
        listaVaciar(&listaPartidas);

    } else if(strcmp(decisionJugador, "B") == 0){

        if(obtenerRanking(&listaRanking, &config)){

            if(!listaVacia(&listaRanking)){
                printf("| Pos | Nombre          | Puntaje Total | Ultima Partida       |\n");
                listaFuncionMap(&listaRanking, mostrarJugadorAPI);
            } else {
                printf("No hay datos cargados en el ranking.\n");
            }

        }

        listaVaciar(&listaRanking);

    }


    return 0;
}
