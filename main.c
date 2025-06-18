#include "juegoPrincipal.h"
#include "listaSimple.h"
#include "informePartidas.h"
int main()
{
    char decisionJugador[MAX_NOMBRE];
    tLista listaJugadores;
    tLista listaPartidas;
    tLista listaRanking;
    tConfiguracion config;
    char nombreArch[20] = "configuracion.txt";
    listaCrear(&listaPartidas);
    listaCrear(&listaRanking);
    crearArchivoConfig(nombreArch);
     do {
        menu(decisionJugador);
        if(strcmp(decisionJugador, "A")== 0){
            obtenerDatosArchivoConfiguracion(nombreArch, &config);
             if(jugar(&listaJugadores, &listaPartidas, config.CantPartidas) == HAY_ERROR){
                printf("Ocurrio un error al ingresar los jugadores.\n");
                return HAY_ERROR;
            }
            ordenarLista(&listaJugadores, compararPuntajeTotal);
            crearInformePartidas(&listaJugadores, &listaPartidas, config.CantPartidas, compararIgualdadPuntajeTotal);
            printf("\nEnviando datos a la API...");
            enviarDatosListaAPI(&listaJugadores, &config, enviarDatosJSON);
            listaVaciar(&listaJugadores);
            listaVaciar(&listaPartidas);
         } else if(strcmp(decisionJugador, "B") == 0){
            obtenerDatosArchivoConfiguracion(nombreArch, &config);
            if(obtenerRanking(&listaRanking, &config)){
                if(!listaVacia(&listaRanking)){
                    printf("| Pos | Nombre          | Puntaje Total | Ultima Partida       |\n");
                    listaFuncionMap(&listaRanking, mostrarJugadorAPI);
                }else {
                    printf("No hay datos cargados en el ranking.\n");
                }
                listaVaciar(&listaRanking);
            }
         }else if(strcmp(decisionJugador, "MODIFICARARCHIVO") == 0){
            modificarArchivoConfig(nombreArch);
         }else if(strcmp(decisionJugador, "C") != 0){
            printf("Opción no válida. Intente nuevamente.\n");

        }
     } while(strcmp(decisionJugador, "C") != 0);
     printf("Saliendo del programa...\n");
    printf("Gracias por jugar!\n");
    return 0;
}
