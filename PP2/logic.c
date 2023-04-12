#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "logic.h"
#include "menu.h"

//tworzy strukture dynamiczna GAMESQUER zwraca na nia wskaünik za parametr przyjmuje koordynaty pixeli dla okna gry
struct GAMESQUARE* creatStructs(int x1, int y1,int x2,int y2) {
    struct GAMESQUARE* ptr = (struct GAMESQUARE*)malloc(sizeof(struct GAMESQUARE));
    if (NULL != ptr) {
        ptr->value = 0;
        ptr->pastValue = 0;
        ptr->x1Pos = x1;
        ptr->y1Pos = y1;
        ptr->x2Pos = x2;
        ptr->y2Pos = y2;
    }
    return ptr;
}

// funkcjia przyjmuje za parametr wskaünik na strukture GAME i uzupelnia tablice BOARD strukturami gamesquer
void creatboard(struct GAME* game) {
    int x1 = 200;
    int y1 = 100;

    int x2 = 300;
    int y2 = 200;

    for (int i = 0;i < 4;i++) {
        y1 += 113;
        y2 += 113;
        for (int j = 0; j < 4;j++) {
            game->board[i][j] = creatStructs(x1, y1,x2,y2);

            x1 += 113;
            x2 += 113;
        }
        x1 = 200;
        x2 = 300;
    }
}

//funkcja zapisuje wartosc obecna po ruch przyjmuje wskaznik struktury GAME
void savePastValues(struct GAME *game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->pastValue = game->board[i][j]->value;
        }
    }

}
//nie uzytkowane
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//nie uzytkowane
void printarr(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf(" %d", game->board[i][j]->value);
        }
        printf("\n");
    }
    printf("\n");
}

//uzupelnia tablice 2d struktur wartosci 0 w celu przygotowania pod nowπ gre
void fillZeros(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->value = 0;
            game->board[i][j]->pastValue = 0;
        }
    }
}

//funkcja losuje pierwsza 2 w tablicy 2d za parametr przyjmuje sie struktur GAME posiadajac tablice struktur z wartosciami gry
void randFirst(struct GAME* game) {
    srand(time(0));
    int i = rand() % 4;
    int j = rand() % 4;
    game->board[i][j]->value = 2;
}

//funckja 
void add(struct GAME* game) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int h = j + 1; h < 4; h++) {
                if (game->board[i][j]->value == game->board[i][h]->value) {
                    game->board[i][j]->value = game->board[i][j]->value * 2;
                    game->score += game->board[i][j]->value;
                    game->board[i][h]->value = 0;
                    break;
                }
                else if (game->board[i][h]->value != 0) {
                    break;
                }
            }
        }
    }
}


void mergeNumInRows(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game->board[i][j]->value == 0) {
                for (int h = j + 1; h < 4; h++) {
                    if (game->board[i][h]->value != 0) {
                        game->board[i][j]->value = game->board[i][h]->value;
                        game->board[i][h]->value = 0;
                        break;
                    }
                }
            }
        }
    }
}

//rotacja 90s w kierunku zegara
void rotateMatrix(struct GAME* game) {
    int i, j, temp;
    for (i = 0; i < 2; i++) {
        for (j = i; j < 4 - i - 1; j++) {
            temp = game->board[i][j]->value;
           game->board[i][j]->value = game->board[4 - j - 1][i]->value;
           game->board[4 - j - 1][i]->value = game->board[4 - i - 1][4 - j - 1]->value;
           game->board[4 - i - 1][4 - j - 1]->value = game->board[j][4 - i - 1]->value;
           game->board[j][4 - i - 1]->value = temp;
        }
    }
}

//nie uzytkowane
void copyMatrix(int copyTab[4][4], int mainTab[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            copyTab[i][j] = mainTab[i][j];
        }
    }
}


bool gameOver(struct GAME* game) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->board[i][j]->value == game->board[i][j+1]->value || game->board[i][j]->value == game->board[i+1][j]->value) {
                return false;
            }
        }
    }
    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 4 ; j++) {
            if (game->board[i][j]->value == 0)
                return false;
        }
    }
    return true;
}


bool lookfor2048(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game->board[i][j]->value == 2048) {
                return true;
            }
        }
    }
    return false;
}
bool compareMatrix(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game->board[i][j]->value != game->board[i][j]->pastValue) {
                return false;
            }
        }
    }
    return true;

}

//do wywalenie
void findEmptyField(int* emptyFieldPointers[15], int tabel[4][4]) {
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tabel[i][j] == 0) {
                emptyFieldPointers[counter] = &tabel[i][j];
                counter++;
            }
        }
    }
}


void moveBoard(struct GAME* game,int direction ) {
    for (int i = 0; i < 4; i++) {
        if (i == direction) {
            add(game);
            mergeNumInRows(game);
        }
        rotateMatrix(game);
    }
}

//do naprawy !!!!!!!!!!!!!!!
void spawn(struct GAME* game) {
    int n = 0;
    int* empty[15];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (game->board[i][j]->value == 0) {
                empty[n] = &game->board[i][j]->value;
                n++;
            }
        }
    }
    printf("%d", n);
    if (n > 0) {
        int index = rand() % n;
        int* p = empty[index];
        *p = 2;
    }
}


void action(struct GAME* game ,int *state ,int key) {
    
    savePastValues(game);
    moveBoard(game,key);
    
    if (compareMatrix(game) == false) {
        spawn(game);
    }
    if (compareMatrix(game) == true && gameOver(game) == true) {
        *state = GAMEOVER;
        save_score(game);
    }
}


void getKey(ALLEGRO_EVENT event, bool *keyUpCheck, int * keyPressed) {
    
    if ( *keyUpCheck == true && event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_A://a
                *keyPressed = A;
                *keyUpCheck = false;
                break;
            case ALLEGRO_KEY_S://s
                *keyPressed = S;
                *keyUpCheck = false;
                break;
            case ALLEGRO_KEY_D://d
                *keyPressed = D;
                *keyUpCheck = false;
                break;
            case ALLEGRO_KEY_W://w
                *keyPressed = W;
                *keyUpCheck = false;
                break;
            default:
                *keyPressed = NONE;
            }
        }
    if (event.type == ALLEGRO_EVENT_KEY_UP) *keyUpCheck = true;
}