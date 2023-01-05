/**
 * @file play.h
 * @author Victoria / Matthieu
 * @brief Fichier header avec l'entete de nos fonctions
 * @version 0.1
 * @date 2022-06-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef H_play
#define H_play

void choix_menu(int event, int x, int y, int f, void* param);

void moteurs(int* pos);

int piano_tiles();

int gameover();

int startmenu();

void choix_game_over(int event, int x, int y, int f, void* param);

void onMouse( int event, int x, int y, int f, void* param);

void init_flags(int* tab,int n);

void gestion_flags(int* tab, int taille, int position_set0);

int jeu();

int jeu_manuel();





#endif