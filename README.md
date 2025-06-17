# Contracampo-TaCTi
Instructivo de Juego TA - C - TI

¿COMO JUGAR?

Al momento de inciar el juego, se despliega un menu donde indica al usuario donde se le da al usuario 2 opciones:

[A] - Jugar: Opcion que permite al jugador jugar el juego TA - C - TI.

[B] - Ver ranking equipo: obtiene el ranking de los equipos almacenados en la base de datos.

[C] - Salir: Permite salir del juego.

--------------------------------------------------------------------------------------------------
[A] - Jugar:

El primer paso para jugar TA - C - TI, es definir los jugadores que seran parte del equipo. Para ello se despliega un menu solicitando los nombres del os jugadores, el juego te permite agregar todos los jugadores que desees. Una vez agregados todos los jugadores ,debes enviar la letra "c" para terminar con la carga.

Una vez cargados los jugadores, comienza el juegon el orden en el que fueron cargados los jugadores.El juego consulta al jugador si esta listado para jugar.  En caso de aceptar, El juego le indica al jugador cuantos juegos le quedan para jugar al jugador y muestra el tablero vacio y se determina de manera random quien empieza jugando, si el jugador o la inteligencia artificial,. En caso de que el jugador empiece, se le asigna la ficha "X", caso contrario, se le asigna la ficha "O".

Posterior a esto el sistema, dependiendo de quien empieza, selecciona determina el orden de los turnos del jugador y de la inteligencia artificial, solicitandole al jugsador seleccionar la fila y columna que desea para poner su ficha en la celda correspondiente,e ncaso de seleccionar una celda no disponible o invalida, el juego solicitara que selecciones una celda valida. Posterior al turno del jugador, la inteligencia artificial realizara su movimiento, buscando superar al jugador.

Los turnos iran pasando hasta que eventualmente el tablero se llene o alguien gane, el sistema dara un puntaje dependiendo de quie ngane y en el caso de que gane el jugador, lo acumulara hasta que finalmente este sea guardado dentro del archivo que sera enviado a la base de datos.

Una vez jugados todos los jugadores, se envia el archivo a la base de datos para poder almacenar el puntaje del equipo.

--------------------------------------------------------------------------------------------------
[B] - Ver ranking equipo:

--------------------------------------------------------------------------------------------------
[C] - Salir:

Permite al usuario dejar de jugar.
