#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>


#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

enum keys {A,S,D,W,NONE,R};

struct GAMESQUARE {
	int value;
	int pastValue;
	int undoMoveValue;
	int x1Pos;
	int y1Pos;
	int x2Pos;
	int y2Pos;
};

struct GAME {
	struct GAMESQUARE* board[4][4];
	int score;
	int undoSocore;
	bool gameWon;
};


struct GAMESQUARE* creatStructs(int x1, int y1, int x2, int y2);
void creatboard(struct GAME* game);
void fillZeros(struct GAME* game);
void randFirst(struct GAME* game);
void add(struct GAME* game);
void mergeNumInRows(struct GAME* game);
void rotateMatrix(struct GAME* game);
bool gameOver(struct GAME* game);
bool lookfor2048(struct GAME* game);
bool compareMatrix(struct GAME* game);
void spawn(struct GAME* game);
void moveBoard(struct GAME* game,int direction);
void action(struct GAME* game, int* state, int key);
void getKey(ALLEGRO_EVENT event, bool* keyUpCheck, int* keyPressed);

#endif // LOGIC_H_INCLUDED
