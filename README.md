# Contracampo-TaCTi
Instructivo de Juego TA - C - TI

Aclaración importante:
Con la intención de que este funcione, debe agregarse las librerias de C:
  <stdlib.h>
	<stdio.h>
	<string.h>
	<time.h>
  <curl/curl.h>
  <stdbool.h>

--------------------------------------------------------------------------------------------------
*Cambiar Configuración*

Con la intención de cambiar la configuración, desde el menu se debe ingresar: "MODIFICARARCHIVO". (Esta opcion tiene la intención de evitar que cualquier jugador casual modifique de forma mal intencionada la configuración, de esta manera solo los usuarios con acceso a este instructivo podran modificar la configuración).

Esto abre un "menu secreto", el cual permite al usuario realizar las siguientes opciones:
- Modicar la URL de la API: Permite modiifcar la url donde se guardaran los resultados.
- Modificar el Codigo de identificacion: Esta opcion permite, como su nombre lo indica, modificar el codigo de identificación
- Modificar la cantidad de partidad: Permine modificar la cantidad de partidad jugadas.

finalmente, en caso de finalizar, se selecciona la opción : "0. Guardar y salir" y te devuelve al menu principal


--------------------------------------------------------------------------------------------------


*¿Como jugar?*

Al momento de inciar el juego, se despliega un menu donde indica al usuario donde se le da al usuario 2 opciones:

[A] - Jugar: Opcion que permite al jugador jugar el juego TA - C - TI.

[B] - Ver ranking equipo: obtiene el ranking de los equipos almacenados en la base de datos.

[C] - Salir: Permite salir del juego.

--------------------------------------
[A] - Jugar:


El primer paso para jugar TA-C-TI consiste en definir los jugadores que formarán parte del equipo. Para ello, se despliega un menú en el que se solicitan los nombres de los jugadores. El juego permite agregar tantos jugadores como se desee. Una vez ingresados todos los nombres, se debe escribir la letra "c" para finalizar la carga.

Con los jugadores ya registrados, el juego inicia siguiendo el orden en que fueron cargados. A cada jugador se le consulta si está disponible para jugar. En caso de que lo este, el juego le informa cuántas partidas le quedan por jugar, posterior a esto, lemuestra el tablero vacío y de forma random decide quién comenzará: el jugador o la inteligencia artificial. Si el jugador es quien inicia, se le asigna la ficha "X"; de lo contrario, se le asigna la ficha "O".

Según quién comience, se establece el orden de los turnos entre el jugador y la inteligencia artificial. El sistema le solicitará al jugador que indique una fila y una columna, para asi indicar la celda donde desea colocar su ficha. Si elige una celda inválida o ya ocupada, se le pedirá que seleccione una celda válida. Luego del turno del jugador, la inteligencia artificial realizará su movimiento, buscando ganar la partida.

Los turnos continuarán alternándose hasta que alguien gane o se complete el tablero. El sistema asignará un puntaje según el resultado: si el jugador gana, su puntaje se acumulará. Al finalizar todas las partidas, este puntaje se guardará en un archivo que será enviado a la base de datos.

Finalmente, una vez que todos los jugadores hayan jugado, el archivo se envía a la base de datos para registrar el puntaje total del equipo.



--------------------------------------
[B] - Ver ranking equipo:

Esta opción muestra el ranking del equipo, donde se ordena a los jugadores por puntaje total de mayor a menor. Se muestran solo los 5 mejores puntajes del equipo.

Se listan:
-  ⁠Nombre del jugador
-  ⁠Puntaje acumulado
-  ⁠Posición en el ranking

Esto permite identificar quién fue el mejor del equipo y comparar rendimientos individuales.

--------------------------------------
[C] - Salir:

Permite al usuario dejar de jugar.
