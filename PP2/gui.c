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

/*
Funkcja intToChar ma za zadanie konwertowaæ wartoœæ liczby ca³kowitej na ci¹g znaków tekstowych. 
Funkcja przyjmuje dwa argumenty: value, który jest wartoœci¹ liczby ca³kowitej do skonwertowania, oraz buffor, który jest wskaŸnikiem na tablicê znaków, do której zostanie zapisany wynik konwersji.
*/
void intToChar (int value,char buffor[8]) {
	snprintf(buffor, sizeof(buffor), "%d", value);
}

/*
Funkcja setColor przyjmuje trzy argumenty : i, j okreœlaj¹ce indeksy w tablicy dwuwymiarowej board oraz wskaŸnik na strukturê GAMESQUARE.
Zwraca wartoœæ typu ALLEGRO_COLOR, która reprezentuje kolor piksela w bibliotece Allegro.
Dzia³anie funkcji polega na sprawdzeniu wartoœci board[i][j]->value i zwróceniu odpowiedniego koloru na podstawie tej wartoœci.
*/
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

/* 
Funkcja printboard przyjmuje argument board, który jest wskaŸnikiem na tablicê dwuwymiarow¹ GAMESQUARE. 
Jej zadaniem jest narysowaæ planszê gry na ekranie przy u¿yciu biblioteki Allegro.
Dzia³anie funkcji polega na przejœciu przez wszystkie elementy tablicy board za pomoc¹ dwóch pêtli for. 
W ka¿dej iteracji, funkcja wywo³uje funkcjê al_draw_filled_rounded_rectangle z odpowiednimi argumentami, aby narysowaæ wype³niony zaokr¹glony prostok¹t na planszy.
*/
void printboard(struct GAMESQUARE *board[4][4]) {
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			al_draw_filled_rounded_rectangle(board[i][j]->x1Pos, board[i][j]->y1Pos, board[i][j]->x2Pos, board[i][j]->y2Pos, 10, 10, setColor(i, j, board));
		}
	}
}

/*
Funkcja printChars przyjmuje dwa argumenty: font, który jest wskaŸnikiem na czcionkê Allegro, oraz game, który jest wskaŸnikiem na strukturê GAME. 
Funkcja ma za zadanie wyœwietliæ wartoœci elementów planszy gry w postaci znaków tekstowych na ekranie.
Dzia³anie funkcji polega na przejœciu przez wszystkie elementy tablicy game->board za pomoc¹ dwóch pêtli for. 
W ka¿dej iteracji, funkcja sprawdza wartoœæ game->board[i][j]->value elementu planszy.
*/
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

/*
Funkcja printScore przyjmuje dwa argumenty: font, który jest wskaŸnikiem na czcionkê Allegro, oraz game, który jest wskaŸnikiem na strukturê GAME. 
Funkcja ma za zadanie wyœwietliæ aktualny wynik gry jako tekst na ekranie.
Dzia³anie funkcji rozpoczyna siê od konwersji wartoœci game->score na ci¹g znaków za pomoc¹ funkcji intToChar(game->score, buffor). 
Buffor to tablica znaków o rozmiarze 7, do której zostanie zapisany wynik konwersji.
*/
void printScore(ALLEGRO_FONT* font , struct GAME *game) {
	char buffor[7];
	intToChar(game->score, buffor);
	al_draw_text(font, al_map_rgb(255, 255, 255), 420, 60, ALLEGRO_ALIGN_CENTRE, buffor);
}

/*
Funkcja winNotification przyjmuje trzy argumenty: game, który jest wskaŸnikiem na strukturê GAME, clock, który jest wskaŸnikiem na zmienn¹ licznika, oraz graphic, który jest wskaŸnikiem na obraz Allegro. 
Funkcja ma na celu wyœwietlenie powiadomienia o wygranej grze na ekranie w przypadku uzyskania wartoœci 2048 na planszy.
Dzia³anie funkcji zaczyna siê od sprawdzenia warunku lookfor2048(game) == true && game->gameWon == false. 
Funkcja lookfor2048 s³u¿y do sprawdzenia, czy na planszy znajduje siê wartoœæ 2048. 
Jeœli ten warunek jest spe³niony, oznacza to, ¿e gracz osi¹gn¹³ wartoœæ 2048 i powiadomienie o wygranej powinno zostaæ wyœwietlone.
*/
void winNotification(struct GAME* game, int* clock,ALLEGRO_BITMAP* graphic) {
	if (lookfor2048(game) == true && game->gameWon == false) {
		al_draw_bitmap(graphic, 200, 205, 0);
		*clock +=1;
		if (*clock > 300) {
			game->gameWon = true;
		}
	}
}