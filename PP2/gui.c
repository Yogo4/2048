#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "gui.h"
#include "logic.h"

//zmiana zmiennej typu int(parametr value) na char, buffor przetrzymuje wartosc zmieniona na char
void intToChar (int value,char buffor[8]) {
	snprintf(buffor, sizeof(buffor), "%d", value);
}

//zwraca color za pomoca funkcji allegro, bazujac na wartosci zawartej w polu struktury board value, parametry i j to inedxy tablicy 
ALLEGRO_COLOR setColor(int i,int j, struct GAMESQUARE* board[4][4]) {
	switch (board[i][j]->value)
	{
	case 0:
		return al_map_rgb(100, 110, 110);
		break;
	case 2:
		return al_map_rgb(238, 228, 218);
		break;
	case 4:
		return al_map_rgb(238, 225, 201);
		break;
	case 8:
		return al_map_rgb(243, 178, 122);
		break;
	case 16:
		return al_map_rgb(247, 150, 100);
		break;
	case 32:
		return al_map_rgb(247, 124, 95);
		break;
	case 64:
		return al_map_rgb(247, 95, 59);
		break;
	case 128:
		return al_map_rgb(237, 208, 115);
		break;
	case 256:
		return al_map_rgb(237, 204, 98);
		break;
	case 512:
		return al_map_rgb(237, 201, 80);
		break;
	case 1024:
		return al_map_rgb(237, 197, 63);
		break;
	case 2048:
		return al_map_rgb(237, 194, 46);
		break;
	default:
		return al_map_rgb(20, 0, 0);
		break;
	}
}

//funkcja interacyjnie przechodzi przez tablice 2d, dla kazdego pola w tablicy board generowany jest kwadrat, bazujac na zmiennych w strukturze
void printboard(struct GAMESQUARE *board[4][4]) {
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			al_draw_filled_rounded_rectangle(board[i][j]->x1Pos, board[i][j]->y1Pos, board[i][j]->x2Pos, board[i][j]->y2Pos, 10, 10, setColor(i, j, board));
		}
	}
}

//funkcja interacyjnie przechodzi przez tablice 2d, sczytuje wartosc z struktury zamienia, wartosc int na char za pomoca funkcji(intToChar), nastepnie wypsuje ta wartosc za pomoca funkcji allegro
void printChars(ALLEGRO_FONT* font,struct GAME* game) {
	char buffor[7];
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			if (game->board[i][j]->value != 0) {
				intToChar(game->board[i][j]->value, buffor);
				al_draw_text(font, al_map_rgb(50, 50, 50), game->board[i][j]->x1Pos + 50, game->board[i][j]->y1Pos +10 , ALLEGRO_ALIGN_CENTRE, buffor);
			}
		}
	}
}

//wypisuje wynik podczas gry zamieniajac wynik (int na char) przy pomocy funkcji allegro
void printScore(ALLEGRO_FONT* font , struct GAME *game) {
	char buffor[7];
	intToChar(game->score, buffor);
	al_draw_text(font, al_map_rgb(255, 255, 255), 420, 60, ALLEGRO_ALIGN_CENTRE, buffor);
}

//funkcja jest uruchamiana po pierwszym 2048, pokazuje komunika przez 300s
void areYouWinningSon(struct GAME* game, int* clock,ALLEGRO_BITMAP* graphic) {
	if (lookfor2048(game) == true && game->gameWon == false) {
		al_draw_bitmap(graphic, 200, 205, 0);
		*clock +=1;
		if (*clock > 300) {
			game->gameWon = true;
		}
	}

}