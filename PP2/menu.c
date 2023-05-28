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
#include "menu.h"

/*
Funkcja set_menu przyjmuje jeden argument: menu, który jest wskaŸnikiem na strukturê MENU. 
Funkcja ma za zadanie ustawienie odpowiednich wartoœci zmiennych w strukturze MENU w celu okreœlenia aktualnego stanu menu gry.
*/
void set_menu(struct MENU* menu) {
	menu->Playing = 1;
	menu->Leadboard = 0;
	menu->QuitGame = 0;
}

/*
Funkcja selectState przyjmuje dwa argumenty: menu, który jest wskaŸnikiem na strukturê MENU, oraz keyPressed, który jest zmienn¹ przechowuj¹c¹ informacjê o naciœniêtym klawiszu. 
Funkcja ma na celu zmianê aktualnego stanu menu w zale¿noœci od naciœniêtego klawisza.
selectState jest wywo³ywana w celu obs³ugi zmiany stanu menu w odpowiedzi na naciœniêcie klawiszy. 
Pozwala to na nawigacjê po menu gry i wybieranie ró¿nych opcji.
*/
void selectState(struct MENU* menu,int keyPressed){
	int tmp;
	
	if (keyPressed == W) {
		tmp = menu->QuitGame;
		menu->QuitGame = menu->Playing;
		menu->Playing = menu->Leadboard;
		menu->Leadboard = tmp;
	}
	else if (keyPressed == S) {
		tmp = menu->Playing;
		menu->Playing = menu->QuitGame;
		menu->QuitGame = menu->Leadboard;
		menu->Leadboard = tmp;
	}
}

/*
Funkcja initState przyjmuje dwa argumenty: menu, który jest wskaŸnikiem na strukturê MENU, oraz menuState, który jest wskaŸnikiem na zmienn¹ przechowuj¹c¹ stan menu. 
Funkcja ma za zadanie ustawiæ odpowiedni stan gry na podstawie wartoœci zmiennych w strukturze MENU.
Dzia³anie funkcji polega na sprawdzeniu wartoœci zmiennych Playing, Leadboard i QuitGame w strukturze menu i na tej podstawie przypisanie odpowiednich wartoœci do zmiennej *menuState.
*/
void initState(struct MENU* menu, int* menuState){
	if (menu->Playing == 1) {
		*menuState = RUN_GAME;
	}
	else if(menu->Leadboard == 1){
		*menuState = LEADBOARD;
	}
	else if (menu->QuitGame == 1) {
		*menuState = QUITGAME;
	}
}

/*
Funkcja printMenu przyjmuje jeden argument: font, który jest wskaŸnikiem na czcionkê typu ALLEGRO_FONT. 
Funkcja ma na celu wyœwietlenie menu gry na ekranie.
W efekcie, funkcja printMenu rysuje trzy opisy opcji menu na ekranie, rozmieszczone pionowo. Ka¿dy opis jest wyœwietlany w wyznaczonym miejscu przy u¿yciu czcionki font. 
Jest to wykorzystywane do wizualizacji menu gry i prezentowania dostêpnych opcji dla gracza.
*/
void printMenu(ALLEGRO_FONT* font) {
	int x = 420;
	int y = 400;
	char desription[3][10] = { {"GAME"},{"LEADBOARD"},{"QUIT"} };

	for (int i = 0; i < 3; i++) {
		al_draw_text(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, desription[i]);
		y += 120;

	}
}

/*
Funkcja printSelected przyjmuje dwa argumenty: menu, który jest wskaŸnikiem na strukturê MENU, oraz font, który jest wskaŸnikiem na czcionkê typu ALLEGRO_FONT. 
Funkcja ma na celu wyœwietlenie wyró¿nionego wyboru w menu gry na ekranie.
*/
void printSelected(struct MENU* menu, ALLEGRO_FONT* font) {	
		if (menu->Playing == 1) {
			al_draw_filled_rounded_rectangle(160, 400, 670, 520, 10, 10, al_map_rgb(255,255,255) );
			al_draw_text(font, al_map_rgb(0, 0, 0), 420, 400, ALLEGRO_ALIGN_CENTRE, "GAME");
		}
		else if(menu->Leadboard == 1){
			al_draw_filled_rounded_rectangle(160, 520, 670, 640, 10, 10, al_map_rgb(255, 255, 255));
			al_draw_text(font, al_map_rgb(0, 0, 0), 410, 520, ALLEGRO_ALIGN_CENTRE, "LEADBOARD");
		}
		else if(menu->QuitGame == 1){
			al_draw_filled_rounded_rectangle(160, 640, 670, 760, 10, 10, al_map_rgb(255, 255, 255));
			al_draw_text(font, al_map_rgb(0, 0, 0), 420, 640, ALLEGRO_ALIGN_CENTRE, "QUIT");
		}
}

/*
Funkcja printScores przyjmuje dwa argumenty: font, który jest wskaŸnikiem na czcionkê typu ALLEGRO_FONT, oraz scores, który jest tablic¹ liczb ca³kowitych o rozmiarze 10. 
Funkcja ma na celu wyœwietlenie listy wyników na ekranie.
printScores rysuje listê wyników na ekranie przy u¿yciu podanej czcionki. 
Wykorzystuje iteracjê, sprawdzanie warunku i przesuniêcie po³o¿enia tekstu, aby wyœwietliæ wyniki w odpowiednich miejscach na ekranie.
Ka¿dy wynik jest numerowany i wyœwietlany jako tekst, co umo¿liwia graczy œledzenie najlepszych wyników.
*/
void printScores(ALLEGRO_FONT* font, int scores[10]) {
	int y = 20;
	char buffor[8];
	char listing[11];
	for (int i = 0; i < 10; i++) {
		if (scores[i] >= 0) {
			intToChar(scores[i], buffor);
			sprintf_s(listing,11,"%d. %s",i+1,buffor);
			al_draw_text(font, al_map_rgb(255, 255, 255), 220, y, ALLEGRO_ALIGN_LEFT, listing);
			y += 78;
		}
	}
}

/*
Funkcja saveScore jest odpowiedzialna za zapisanie wyniku gry do pliku leadboard. Przyjmuje wskaŸnik game na strukturê GAME, która przechowuje aktualny wynik gry.
saveScore odczytuje tablicê wyników z pliku, porównuje aktualny wynik gry z wynikami w tablicy, aktualizuje tablicê i zapisuje j¹ z powrotem do pliku. 
Pozwala to na przechowywanie i aktualizowanie najlepszych wyników w grze.
*/
void saveScore(struct GAME *game) {
	FILE* leadboard;
	char filename[] = "leadboard.txt";
	errno_t  err = fopen_s(&leadboard, filename, "r+");
	
	int i, j;
	int scores[10];

	if (leadboard == NULL) {
		printf("Error: Unable to open file %s.\n", filename);
		exit(1);
	}

	for (i = 0; i < 10; i++) {
		fscanf_s(leadboard, "%d", &scores[i]);
	}

	for (i = 0; i < 10; i++) {
		if (game->score > scores[i]) {
			for (j = 10 - 1; j > i; j--) {
				scores[j] = scores[j - 1];
			}

			scores[i] = game->score;

			rewind(leadboard);
			for (j = 0; j < 10; j++) {
				fprintf_s(leadboard, "%d\n", scores[j]);
			}
			fflush(leadboard);
			break;
		}
	}
	fclose(leadboard);
}


/*
Funkcja read_scores jest odpowiedzialna za odczytanie wyników z pliku leadboard do tablicy scores.
Przyjmuje tablicê scores o rozmiarze 10 jako argument, do której zostan¹ zapisane odczytane wyniki.
*/
void readScores(int scores[10]) {
	FILE* leadboard;
	char filename[] = "leadboard.txt";
	errno_t  err = fopen_s(&leadboard, filename, "r+");

	int i;
	if (leadboard == NULL) {
		exit(1);
	}
	for (i = 0; i < 10; i++) {
		fscanf_s(leadboard, "%d", &scores[i]);
	}

	fclose(leadboard);
}