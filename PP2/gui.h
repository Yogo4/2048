#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

void intToChar(int value, char buffor[8]);
ALLEGRO_COLOR setColor(int i, int j, struct GAMESQUARE* board[4][4]);
void printboard(struct GAMESQUARE *board[4][4]);
void printChars(ALLEGRO_FONT* font, struct GAME* game);
void printScore(ALLEGRO_FONT* font, struct GAME* game);
void winNotification(struct GAME* game, int* clock, ALLEGRO_BITMAP* graphic);
#endif // GUI_H_INCLUDED
