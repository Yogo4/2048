#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "logic.h"
#include "gui.h"
#include "menu.h"
#include <windows.h>

int main(void) {


	int height = 840;
	int width = 840;
	

	struct MENU menu;
	set_menu(&menu);

	struct GAME game;
	game.score = 0;
	game.gameWon = false;
	creatboard(&game);
	
	int state = IN_MENU;
	int menuState = REST;
	
	int scores[10];

	int keyPressed = NONE;
	bool keyUpCheck = true;

	int FPSlimit = 60;
	time_t t;
	int promptClock = 0;

	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	srand((unsigned)time(&t));

	ALLEGRO_COLOR bgcolor = al_map_rgb(60, 58, 51);
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_FONT* helpfont = al_load_font("ClearSans-Medium.ttf", 15, 0);
	ALLEGRO_FONT* font = al_load_font("ClearSans-Medium.ttf",50,0);
	ALLEGRO_FONT* menuFont = al_load_font("ClearSans-Medium.ttf", 80, 0);
	ALLEGRO_FONT* menuSeleced = al_load_font("ClearSans-Medium.ttf", 90, 0);
	ALLEGRO_BITMAP* gameover = al_load_bitmap("gameover.png");
	ALLEGRO_BITMAP* gamewon = al_load_bitmap("gamewon.png");


	display = al_create_display( width, height);
	timer = al_create_timer(1.0 / FPSlimit);
	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	

	randFirst(&game);
		
	al_start_timer(timer);
	while (1) {
		
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);
		
		if (state == IN_MENU) {
			if (menuState == REST) {
				if (event.type == ALLEGRO_EVENT_TIMER) {
					printMenu(menuFont);
					printSelected(&menu, menuSeleced);
					al_draw_text(helpfont, al_map_rgb(255, 255, 255), 835, 0, ALLEGRO_ALIGN_RIGHT, "move(a,w,s,d)");
					al_draw_text(helpfont, al_map_rgb(255, 255, 255), 835, 18, ALLEGRO_ALIGN_RIGHT, "confirm(enter)");
					al_flip_display();
				}
				getKey(event, &keyUpCheck, &keyPressed);
				selectState(&menu, keyPressed);

				keyPressed = NONE;
				al_clear_to_color(bgcolor);
			}

			if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				initState(&menu, &menuState);
			}

			if (menuState == RUN_GAME) {
				al_clear_to_color(bgcolor);
				al_flip_display();
				state = PLAYING;
			}
			else if (menuState == LEADBOARD) {
				read_scores(scores);
				if (event.type == ALLEGRO_EVENT_TIMER) {
					al_clear_to_color(bgcolor);
					printScores(menuFont, &scores);
					al_draw_text(helpfont, al_map_rgb(255, 255, 255), 835, 0, ALLEGRO_ALIGN_RIGHT, "menu(esc)");
					al_flip_display();
				}
				if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					menuState = REST;
					set_menu(&menu);
				}
			}
			else if (menuState == QUITGAME) {
				save_score(&game);
				return 1;
			}
		}

		if (state == PLAYING) {
			//game.board[0][0]->value = 2048;
			getKey(event, &keyUpCheck, &keyPressed);
			printf("\n%d\n", keyPressed);
			action(&game,&state, keyPressed);
			printarr(&game);
			if (event.type == ALLEGRO_EVENT_TIMER) {
				printboard(&game.board);
				printChars(font, &game);
				printScore(menuFont, &game);
				areYouWinningSon(&game, &promptClock, gamewon);
				al_draw_text(helpfont,al_map_rgb(255,255,255),835,0,ALLEGRO_ALIGN_RIGHT,"move(a,w,s,d)");
				al_draw_text(helpfont, al_map_rgb(255, 255, 255), 835, 18, ALLEGRO_ALIGN_RIGHT, "menu(esc)");
				al_flip_display();
			}
			keyPressed = NONE;
			al_clear_to_color(bgcolor);
			
			if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				menuState = REST;
				set_menu(&menu);
				state = IN_MENU;
			}
		}

		if (state == GAMEOVER) {
			if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				game.gameWon = false;
				game.score = 0;
				set_menu(&menu);
				state = IN_MENU;
				menuState = REST;
				fillZeros(&game);
				randFirst(&game);
			}
			if (event.type == ALLEGRO_EVENT_TIMER) {
				printboard(&game.board);
				printChars(font, &game);
				al_draw_bitmap(gameover, 200, 205, 0);
				printScore(menuFont, &game);
				al_flip_display();
			}
		}
	}
	al_destroy_display(display);
}