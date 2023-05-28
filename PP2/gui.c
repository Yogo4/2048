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
Funkcja intToChar ma za zadanie konwertowa� warto�� liczby ca�kowitej na ci�g znak�w tekstowych. 
Funkcja przyjmuje dwa argumenty: value, kt�ry jest warto�ci� liczby ca�kowitej do skonwertowania, oraz buffor, kt�ry jest wska�nikiem na tablic� znak�w, do kt�rej zostanie zapisany wynik konwersji.
*/
void intToChar (int value,char buffor[8]) {
	snprintf(buffor, sizeof(buffor), "%d", value);
}

/*
Funkcja setColor przyjmuje trzy argumenty : i, j okre�laj�ce indeksy w tablicy dwuwymiarowej board oraz wska�nik na struktur� GAMESQUARE.
Zwraca warto�� typu ALLEGRO_COLOR, kt�ra reprezentuje kolor piksela w bibliotece Allegro.
Dzia�anie funkcji polega na sprawdzeniu warto�ci board[i][j]->value i zwr�ceniu odpowiedniego koloru na podstawie tej warto�ci.
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
Funkcja printboard przyjmuje argument board, kt�ry jest wska�nikiem na tablic� dwuwymiarow� GAMESQUARE. 
Jej zadaniem jest narysowa� plansz� gry na ekranie przy u�yciu biblioteki Allegro.
Dzia�anie funkcji polega na przej�ciu przez wszystkie elementy tablicy board za pomoc� dw�ch p�tli for. 
W ka�dej iteracji, funkcja wywo�uje funkcj� al_draw_filled_rounded_rectangle z odpowiednimi argumentami, aby narysowa� wype�niony zaokr�glony prostok�t na planszy.
*/
void printboard(struct GAMESQUARE *board[4][4]) {
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			al_draw_filled_rounded_rectangle(board[i][j]->x1Pos, board[i][j]->y1Pos, board[i][j]->x2Pos, board[i][j]->y2Pos, 10, 10, setColor(i, j, board));
		}
	}
}

/*
Funkcja printChars przyjmuje dwa argumenty: font, kt�ry jest wska�nikiem na czcionk� Allegro, oraz game, kt�ry jest wska�nikiem na struktur� GAME. 
Funkcja ma za zadanie wy�wietli� warto�ci element�w planszy gry w postaci znak�w tekstowych na ekranie.
Dzia�anie funkcji polega na przej�ciu przez wszystkie elementy tablicy game->board za pomoc� dw�ch p�tli for. 
W ka�dej iteracji, funkcja sprawdza warto�� game->board[i][j]->value elementu planszy.
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
Funkcja printScore przyjmuje dwa argumenty: font, kt�ry jest wska�nikiem na czcionk� Allegro, oraz game, kt�ry jest wska�nikiem na struktur� GAME. 
Funkcja ma za zadanie wy�wietli� aktualny wynik gry jako tekst na ekranie.
Dzia�anie funkcji rozpoczyna si� od konwersji warto�ci game->score na ci�g znak�w za pomoc� funkcji intToChar(game->score, buffor). 
Buffor to tablica znak�w o rozmiarze 7, do kt�rej zostanie zapisany wynik konwersji.
*/
void printScore(ALLEGRO_FONT* font , struct GAME *game) {
	char buffor[7];
	intToChar(game->score, buffor);
	al_draw_text(font, al_map_rgb(255, 255, 255), 420, 60, ALLEGRO_ALIGN_CENTRE, buffor);
}

/*
Funkcja winNotification przyjmuje trzy argumenty: game, kt�ry jest wska�nikiem na struktur� GAME, clock, kt�ry jest wska�nikiem na zmienn� licznika, oraz graphic, kt�ry jest wska�nikiem na obraz Allegro. 
Funkcja ma na celu wy�wietlenie powiadomienia o wygranej grze na ekranie w przypadku uzyskania warto�ci 2048 na planszy.
Dzia�anie funkcji zaczyna si� od sprawdzenia warunku lookfor2048(game) == true && game->gameWon == false. 
Funkcja lookfor2048 s�u�y do sprawdzenia, czy na planszy znajduje si� warto�� 2048. 
Je�li ten warunek jest spe�niony, oznacza to, �e gracz osi�gn�� warto�� 2048 i powiadomienie o wygranej powinno zosta� wy�wietlone.
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