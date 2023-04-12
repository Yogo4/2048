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

//ustawia bazowy stan menu 1 to zaznaczony stan przyjmuje za parametr stucture menu
void set_menu(struct MENU* menu) {
	menu->Playing = 1;
	menu->Leadboard = 0;
	menu->QuitGame = 0;
}

//odpowiada za zmian zaznczonego sanu lda struktury menu przy odpowiedni przycisku(w lub s) w parametrze 
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

//inicjalizuje stan zaznaczony w strukturze menu, i zmienia stan w parametrze menuState
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

//odpowiada za wypisanie menu za parametr przyjmuje font 
void printMenu(ALLEGRO_FONT* font) {
	int x = 420;
	int y = 400;
	char desription[3][10] = { {"GAME"},{"LEADBOARD"},{"QUIT"} };

	for (int i = 0; i < 3; i++) {
		al_draw_text(font, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, desription[i]);
		y += 120;

	}

}


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


void save_score(struct GAME *game) {
	FILE* leadboard;
	char filename[] = "leadboard.txt";
	errno_t  err = fopen_s(&leadboard, filename, "r+");
	
	int i, j;
	int scores[10];

	if (leadboard == NULL) {
		printf("Error: Unable to open file %s.\n", filename);
		exit(1);
	}

	// Read the existing scores from the file.
	for (i = 0; i < 10; i++) {
		fscanf_s(leadboard, "%d", &scores[i]);
	}

	// Check if the new score is higher than any of the existing scores.
	for (i = 0; i < 10; i++) {
		if (game->score > scores[i]) {
			// Shift the lower scores down to make room for the new score.
			for (j = 10 - 1; j > i; j--) {
				scores[j] = scores[j - 1];
			}

			// Insert the new score at the correct position.
			scores[i] = game->score;

			// Write the updated scores to the file.
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

void read_scores(int scores[10]) {
	FILE* leadboard;
	char filename[] = "leadboard.txt";
	errno_t  err = fopen_s(&leadboard, filename, "r+");

	int i;
	if (leadboard == NULL) {
		printf("Error: Unable to open file %s.\n", filename);
		exit(1);
	}
	// Read the top 10 scores from the file.
	for (i = 0; i < 10; i++) {
		fscanf_s(leadboard, "%d", &scores[i]);
	}

	// Close the file.
	fclose(leadboard);
}