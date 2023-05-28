#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "logic.h"
#include "menu.h"

/*
Funkcja creatStructs jest odpowiedzialna za dynamiczne tworzenie i inicjalizacj� struktury GAMESQUARE. 
Przyjmuje cztery argumenty: x1, y1, x2 i y2, kt�re okre�laj� pozycj� i wymiary struktury.
*/
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

/*
Funkcja creatboard jest odpowiedzialna za tworzenie planszy gry w strukturze GAME. Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji rozpoczyna si� od inicjalizacji zmiennych x1, y1, x2 i y2 jako pocz�tkowe warto�ci pozycji i wymiar�w struktury GAMESQUARE. 
Te warto�ci zostan� u�yte do tworzenia kolejnych element�w planszy.
*/
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

/*
Funkcja savePastValues jest odpowiedzialna za zapisanie warto�ci poprzednich element�w planszy do ich odpowiednich p�l pastValue w strukturze GAME. 
Przyjmuje wska�nik game na struktur� GAME jako argument.
Funkcja savePastValues jest przydatna w kontek�cie gry, gdy chcemy zapisywa� warto�ci poprzednie element�w planszy, aby m�c por�wna� je p�niej.
*/
void savePastValues(struct GAME *game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->pastValue = game->board[i][j]->value;
        }
    }
}

/*
Funkcja saveValueToUndo jest odpowiedzialna za zapisanie warto�ci element�w planszy przed wykonaniem ruchu do p�l undoMoveValue w strukturze GAME. 
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji rozpoczyna si� od zapisania aktualnej warto�ci score z pola game->score do pola undoScore w celu zachowania jej dla p�niejszego cofni�cia ruchu.
*/
void saveValueToUndo(struct GAME* game) {
    game->undoSocore = game->score;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->undoMoveValue = game->board[i][j]->value;
        }
    }
}

/*
Funkcja undoMove s�u�y do cofania ostatniego wykonanego ruchu w grze.
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na przywr�ceniu warto�ci element�w planszy oraz wyniku gry (score) do ich stanu przed wykonaniem ostatniego ruchu.
*/
void undoMove(struct GAME* game) {
    game->score = game->undoSocore;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->value = game->board[i][j]->undoMoveValue;
        }
    }
}


/*
Funkcja fillZeros s�u�y do wype�nienia wszystkich p�l planszy warto�ci� zero.
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na iteracji przez wszystkie elementy planszy za pomoc� dw�ch p�tli for.
W ka�dej iteracji, warto�� value oraz pastValue tego elementu jest ustawiana na zero. Oznacza to, �e wszystkie pola planszy s� wype�niane zerami.
*/
void fillZeros(struct GAME* game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->value = 0;
            game->board[i][j]->pastValue = 0;
        }
    }
}

/*
Funkcja randFirst s�u�y do wylosowania pocz�tkowej warto�ci 2 na losowym polu planszy. 
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na wywo�aniu funkcji srand z argumentem time(0), co ustawia ziarno generatora liczb pseudolosowych na aktualny czas.
Nast�pnie generowane s� losowe liczby i i j z zakresu od 0 do 3 za pomoc� funkcji rand() % 4.
*/
void randFirst(struct GAME* game) {
    srand(time(0));
    int i = rand() % 4;
    int j = rand() % 4;
    game->board[i][j]->value = 2;
}

/*
Funkcja add s�u�y do sumowania i ��czenia dw�ch s�siednich p�l o tej samej warto�ci w obr�bie tego samego wiersza planszy. 
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na dwukrotnym zagnie�d�eniu p�tli for w celu iteracji przez wszystkie pola planszy. 
Wewn�trz najbardziej zagnie�d�onej p�tli for znajduje si� dodatkowa p�tla for, kt�ra iteruje od bie��cej kolumny do ko�ca wiersza.
Funkcja add jest u�ywana po wykonaniu ruchu w grze w celu po��czenia i sumowania s�siednich p�l o takiej samej warto�ci, co jest jednym z podstawowych element�w mechaniki gry 2048.
*/
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

/*
Funkcja mergeNumInRows s�u�y do przesuwania i scalania warto�ci p�l w obr�bie ka�dego wiersza planszy. 
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na dwukrotnym zagnie�d�eniu p�tli for w celu iteracji przez wszystkie pola planszy. 
Wewn�trz najbardziej zagnie�d�onej p�tli for znajduje si� dodatkowa p�tla for, kt�ra iteruje od bie��cej kolumny do ko�ca wiersza.
Funkcja mergeNumInRows jest u�ywana po wykonaniu ruchu w grze w celu przesuni�cia i scalenia p�l w jednym wierszu planszy.
*/
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

/*
Funkcja rotateMatrix s�u�y do obracania macierzy planszy gry o 90 stopni w kierunku zgodnym z ruchem wskaz�wek zegara. 
Przyjmuje wska�nik game na struktur� GAME jako argument.

Dzia�anie funkcji polega na zastosowaniu algorytmu zamiany miejscami odpowiednich element�w macierzy. 
Zastosowano dwukrotnie zagnie�d�one p�tle for, kt�re iteruj� po elementach macierzy w odpowiednich zakresach.
Funkcja jest u�ywana w celu obs�ugi ruch�w.
*/
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

/*
Funkcja gameOver s�u�y do sprawdzania, czy gra si� zako�czy�a, czyli czy nie ma mo�liwo�ci wykonania kolejnego ruchu.
Przyjmuje wska�nik game na struktur� GAME jako argument i zwraca warto�� logiczn� true lub false.

Funkcja sk�ada si� z dw�ch g��wnych cz�ci:
    -sprawdzanie s�siednich element�w
    -sprawdzanie pustych p�l
Je�li �aden z warunk�w zako�czenia gry nie jest spe�niony, oznacza to, �e nie ma mo�liwo�ci wykonania kolejnego ruchu ani pustych p�l na planszy, co oznacza koniec gry. 
W takim przypadku funkcja zwraca warto�� logiczn� true, sygnalizuj�c zako�czenie gry.
*/
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

/*
Funkcja lookfor2048 s�u�y do sprawdzania, czy na planszy gry znajduje si� element o warto�ci 2048. 
Przyjmuje wska�nik game na struktur� GAME jako argument i zwraca warto�� logiczn� true lub false.
*/
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

/*
Funkcja compareMatrix s�u�y do por�wnywania bie��cej macierzy warto�ci planszy gry z macierz� poprzednich warto�ci.
Przyjmuje wska�nik game na struktur� GAME jako argument i zwraca warto�� logiczn� true lub false.
*/
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

/*
Funkcja moveBoard s�u�y do przesuwania planszy gry w okre�lonym kierunku. 
Przyjmuje wska�nik game na struktur� GAME oraz warto�� direction, kt�ra okre�la kierunek przesuni�cia (0 - w lewo, 1 - w g�r�, 2 - w prawo, 3 - w d�).

Dzia�anie funkcji polega na wykonaniu operacji przesuni�cia planszy w danym kierunku. 
Najpierw wywo�ywana jest funkcja saveValueToUndo(game), kt�ra zapisuje warto�ci planszy i wynik przed przesuni�ciem, umo�liwiaj�c p�niejsze cofni�cie ruchu za pomoc� funkcji undoMove.
Operacja dodawania polega na wywo�aniu funkcji add(game), kt�ra sprawdza i ��czy pary s�siednich liczb o takiej samej warto�ci.
Operacja ��czenia liczb w wierszach polega na wywo�aniu funkcji mergeNumInRows(game), kt�ra przesuwa liczby w wierszach, ��cz�c je, je�li s�siaduj�ce maj� tak� sam� warto��.
*/
void moveBoard(struct GAME* game,int direction ) {
    if (direction < 4) {
        saveValueToUndo(game);
        for (int i = 0; i < 4; i++) {
            if (i == direction) {
                add(game);
                mergeNumInRows(game);
            }
            rotateMatrix(game);
        }
    }
}

/*
Funkcja spawn s�u�y do pojawiania si� nowych liczb na planszy gry.
Przyjmuje wska�nik game na struktur� GAME.

Dzia�anie funkcji polega na przeszukiwaniu planszy w poszukiwaniu pustych miejsc, gdzie warto�� liczb wynosi 0. 
Ka�de takie puste miejsce jest reprezentowane przez wska�nik na warto�� liczby (wska�nik na value) i jest przechowywane w tablicy wska�nik�w empty.
*/
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
    if (n > 0) {
        int index = rand() % n;
        int* p = empty[index];
        *p = 2;
    }
}

/*
Funkcja action odpowiada za wykonanie akcji na planszy gry na podstawie podanego klawisza. 
Przyjmuje wska�nik game na struktur� GAME, wska�nik state na zmienn� stanu gry oraz warto�� key reprezentuj�c� wci�ni�ty klawisz.

Dzia�anie funkcji jest nast�puj�ce:
    Je�li wci�ni�ty klawisz (key) jest r�wny 5, wywo�ywana jest funkcja undoMove w celu cofni�cia ostatniego ruchu.
    Nast�pnie, wywo�ywana jest funkcja savePastValues w celu zapisania warto�ci poprzednich p�l na planszy.
    Kolejnym krokiem jest wywo�anie funkcji moveBoard z przekazaniem warto�ci key w celu przesuni�cia planszy w odpowiednim kierunku.
    Je�li funkcja compareMatrix zwraca warto�� false, oznacza to, �e plansza uleg�a zmianie w wyniku wykonanej akcji. W takim przypadku wywo�ywana jest funkcja spawn w celu pojawienia si� nowej liczby na planszy.
    Je�li funkcja compareMatrix zwraca warto�� true (plansza nie uleg�a zmianie) oraz funkcja gameOver zwraca warto�� true (koniec gry), to zmienna state jest ustawiana na warto�� GAMEOVER (oznaczaj�c� koniec gry) i wywo�ywana jest funkcja saveScore w celu zapisania aktualnego wyniku gry.
*/
void action(struct GAME* game ,int *state ,int key) {
    if (key == 5) {
        undoMove(game);
    }
    savePastValues(game);
    moveBoard(game,key);

    if (compareMatrix(game) == false) {   
        spawn(game);
    }
    if (compareMatrix(game) == true && gameOver(game) == true) {
        *state = GAMEOVER;
        saveScore(game);
    }
}

/*
Funkcja getKey odpowiada za odczytanie wci�ni�tego klawisza zdarzenia event w kontek�cie biblioteki Allegro. 
Przyjmuje wska�nik keyUpCheck na zmienn� logiczn� sprawdzaj�c�, czy klawisz zosta� zwolniony, oraz wska�nik keyPressed na zmienn� przechowuj�c� warto�� odczytanego klawisza.
*/
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
            case ALLEGRO_KEY_R://r
                *keyPressed = R;
                *keyUpCheck = false;
                break;
            default:
                *keyPressed = NONE;
            }
        }
    if (event.type == ALLEGRO_EVENT_KEY_UP) *keyUpCheck = true;
}