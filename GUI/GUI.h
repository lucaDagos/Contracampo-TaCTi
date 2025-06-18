//
// Created by Julieta Moreno on 17/06/2025.
//

#ifndef GUI_H
#define GUI_H
#ifdef __cplusplus
extern "C" {
#endif

    // ——— Gestión de jugadores y ranking ———
    void    tt_init_players(void);
    int     tt_add_player(char *name);
    int     tt_get_num_players(void);
    char*   tt_select_random_player(void){
    int    tt_get_num_games();
    // Rellena arrays pre-alocados de tamaño max; retorna jugadores reales

    // ——— Juego individual contra IA ———
    void    tt_game_init(void);
    // Devuelve 'X','O' o espacio en blanco
    char    tt_get_cell(int row, int col);
    // MOV_REALIZADO=1 o MOV_NO_REALIZADO=-1
    int     tt_player_move(int row, int col);
    int     tt_ai_move(void);
    // 0=en curso,1=jugador,2=IA,3=empate
    int     tt_check_status(void);

#ifdef __cplusplus
}
#endif
#endif //GUI_H
