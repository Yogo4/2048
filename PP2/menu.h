#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

enum gamestate { IN_MENU, PLAYING, GAMEOVER };
enum menuState { RUN_GAME, LEADBOARD, QUITGAME, REST };

struct MENU
{
	int  Playing;
	int  Leadboard;
	int QuitGame;
};

void set_menu(struct MENU* menu);
void selectState(struct MENU* gameMenu, int keyPressed);
void initState(struct MENU* gameMenu, int* menuState);
void printMenu(ALLEGRO_FONT* font);
void printSelected(struct MENU* menu, ALLEGRO_FONT* font);
void printScores(ALLEGRO_FONT* font, int scores[10]);
void saveScore(struct GAME* game);
void readScores(int scores[10]);

#endif // MENU_H_INCLUDED
