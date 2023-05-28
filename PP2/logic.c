#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "logic.h"
#include "menu.h"

/*
Funkcja creatStructs jest odpowiedzialna za dynamiczne tworzenie i inicjalizacjê struktury GAMESQUARE. 
Przyjmuje cztery argumenty: x1, y1, x2 i y2, które okreœlaj¹ pozycjê i wymiary struktury.
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
Funkcja creatboard jest odpowiedzialna za tworzenie planszy gry w strukturze GAME. Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji rozpoczyna siê od inicjalizacji zmiennych x1, y1, x2 i y2 jako pocz¹tkowe wartoœci pozycji i wymiarów struktury GAMESQUARE. 
Te wartoœci zostan¹ u¿yte do tworzenia kolejnych elementów planszy.
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
Funkcja savePastValues jest odpowiedzialna za zapisanie wartoœci poprzednich elementów planszy do ich odpowiednich pól pastValue w strukturze GAME. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.
Funkcja savePastValues jest przydatna w kontekœcie gry, gdy chcemy zapisywaæ wartoœci poprzednie elementów planszy, aby móc porównaæ je póŸniej.
*/
void savePastValues(struct GAME *game) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            game->board[i][j]->pastValue = game->board[i][j]->value;
        }
    }
}

/*
Funkcja saveValueToUndo jest odpowiedzialna za zapisanie wartoœci elementów planszy przed wykonaniem ruchu do pól undoMoveValue w strukturze GAME. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji rozpoczyna siê od zapisania aktualnej wartoœci score z pola game->score do pola undoScore w celu zachowania jej dla póŸniejszego cofniêcia ruchu.
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
Funkcja undoMove s³u¿y do cofania ostatniego wykonanego ruchu w grze.
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na przywróceniu wartoœci elementów planszy oraz wyniku gry (score) do ich stanu przed wykonaniem ostatniego ruchu.
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
Funkcja fillZeros s³u¿y do wype³nienia wszystkich pól planszy wartoœci¹ zero.
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na iteracji przez wszystkie elementy planszy za pomoc¹ dwóch pêtli for.
W ka¿dej iteracji, wartoœæ value oraz pastValue tego elementu jest ustawiana na zero. Oznacza to, ¿e wszystkie pola planszy s¹ wype³niane zerami.
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
Funkcja randFirst s³u¿y do wylosowania pocz¹tkowej wartoœci 2 na losowym polu planszy. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na wywo³aniu funkcji srand z argumentem time(0), co ustawia ziarno generatora liczb pseudolosowych na aktualny czas.
Nastêpnie generowane s¹ losowe liczby i i j z zakresu od 0 do 3 za pomoc¹ funkcji rand() % 4.
*/
void randFirst(struct GAME* game) {
    srand(time(0));
    int i = rand() % 4;
    int j = rand() % 4;
    game->board[i][j]->value = 2;
}

/*
Funkcja add s³u¿y do sumowania i ³¹czenia dwóch s¹siednich pól o tej samej wartoœci w obrêbie tego samego wiersza planszy. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na dwukrotnym zagnie¿d¿eniu pêtli for w celu iteracji przez wszystkie pola planszy. 
Wewn¹trz najbardziej zagnie¿d¿onej pêtli for znajduje siê dodatkowa pêtla for, która iteruje od bie¿¹cej kolumny do koñca wiersza.
Funkcja add jest u¿ywana po wykonaniu ruchu w grze w celu po³¹czenia i sumowania s¹siednich pól o takiej samej wartoœci, co jest jednym z podstawowych elementów mechaniki gry 2048.
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
Funkcja mergeNumInRows s³u¿y do przesuwania i scalania wartoœci pól w obrêbie ka¿dego wiersza planszy. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na dwukrotnym zagnie¿d¿eniu pêtli for w celu iteracji przez wszystkie pola planszy. 
Wewn¹trz najbardziej zagnie¿d¿onej pêtli for znajduje siê dodatkowa pêtla for, która iteruje od bie¿¹cej kolumny do koñca wiersza.
Funkcja mergeNumInRows jest u¿ywana po wykonaniu ruchu w grze w celu przesuniêcia i scalenia pól w jednym wierszu planszy.
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
Funkcja rotateMatrix s³u¿y do obracania macierzy planszy gry o 90 stopni w kierunku zgodnym z ruchem wskazówek zegara. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument.

Dzia³anie funkcji polega na zastosowaniu algorytmu zamiany miejscami odpowiednich elementów macierzy. 
Zastosowano dwukrotnie zagnie¿d¿one pêtle for, które iteruj¹ po elementach macierzy w odpowiednich zakresach.
Funkcja jest u¿ywana w celu obs³ugi ruchów.
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
Funkcja gameOver s³u¿y do sprawdzania, czy gra siê zakoñczy³a, czyli czy nie ma mo¿liwoœci wykonania kolejnego ruchu.
Przyjmuje wskaŸnik game na strukturê GAME jako argument i zwraca wartoœæ logiczn¹ true lub false.

Funkcja sk³ada siê z dwóch g³ównych czêœci:
    -sprawdzanie s¹siednich elementów
    -sprawdzanie pustych pól
Jeœli ¿aden z warunków zakoñczenia gry nie jest spe³niony, oznacza to, ¿e nie ma mo¿liwoœci wykonania kolejnego ruchu ani pustych pól na planszy, co oznacza koniec gry. 
W takim przypadku funkcja zwraca wartoœæ logiczn¹ true, sygnalizuj¹c zakoñczenie gry.
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
Funkcja lookfor2048 s³u¿y do sprawdzania, czy na planszy gry znajduje siê element o wartoœci 2048. 
Przyjmuje wskaŸnik game na strukturê GAME jako argument i zwraca wartoœæ logiczn¹ true lub false.
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
Funkcja compareMatrix s³u¿y do porównywania bie¿¹cej macierzy wartoœci planszy gry z macierz¹ poprzednich wartoœci.
Przyjmuje wskaŸnik game na strukturê GAME jako argument i zwraca wartoœæ logiczn¹ true lub false.
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
Funkcja moveBoard s³u¿y do przesuwania planszy gry w okreœlonym kierunku. 
Przyjmuje wskaŸnik game na strukturê GAME oraz wartoœæ direction, która okreœla kierunek przesuniêcia (0 - w lewo, 1 - w górê, 2 - w prawo, 3 - w dó³).

Dzia³anie funkcji polega na wykonaniu operacji przesuniêcia planszy w danym kierunku. 
Najpierw wywo³ywana jest funkcja saveValueToUndo(game), która zapisuje wartoœci planszy i wynik przed przesuniêciem, umo¿liwiaj¹c póŸniejsze cofniêcie ruchu za pomoc¹ funkcji undoMove.
Operacja dodawania polega na wywo³aniu funkcji add(game), która sprawdza i ³¹czy pary s¹siednich liczb o takiej samej wartoœci.
Operacja ³¹czenia liczb w wierszach polega na wywo³aniu funkcji mergeNumInRows(game), która przesuwa liczby w wierszach, ³¹cz¹c je, jeœli s¹siaduj¹ce maj¹ tak¹ sam¹ wartoœæ.
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
Funkcja spawn s³u¿y do pojawiania siê nowych liczb na planszy gry.
Przyjmuje wskaŸnik game na strukturê GAME.

Dzia³anie funkcji polega na przeszukiwaniu planszy w poszukiwaniu pustych miejsc, gdzie wartoœæ liczb wynosi 0. 
Ka¿de takie puste miejsce jest reprezentowane przez wskaŸnik na wartoœæ liczby (wskaŸnik na value) i jest przechowywane w tablicy wskaŸników empty.
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
Przyjmuje wskaŸnik game na strukturê GAME, wskaŸnik state na zmienn¹ stanu gry oraz wartoœæ key reprezentuj¹c¹ wciœniêty klawisz.

Dzia³anie funkcji jest nastêpuj¹ce:
    Jeœli wciœniêty klawisz (key) jest równy 5, wywo³ywana jest funkcja undoMove w celu cofniêcia ostatniego ruchu.
    Nastêpnie, wywo³ywana jest funkcja savePastValues w celu zapisania wartoœci poprzednich pól na planszy.
    Kolejnym krokiem jest wywo³anie funkcji moveBoard z przekazaniem wartoœci key w celu przesuniêcia planszy w odpowiednim kierunku.
    Jeœli funkcja compareMatrix zwraca wartoœæ false, oznacza to, ¿e plansza uleg³a zmianie w wyniku wykonanej akcji. W takim przypadku wywo³ywana jest funkcja spawn w celu pojawienia siê nowej liczby na planszy.
    Jeœli funkcja compareMatrix zwraca wartoœæ true (plansza nie uleg³a zmianie) oraz funkcja gameOver zwraca wartoœæ true (koniec gry), to zmienna state jest ustawiana na wartoœæ GAMEOVER (oznaczaj¹c¹ koniec gry) i wywo³ywana jest funkcja saveScore w celu zapisania aktualnego wyniku gry.
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
Funkcja getKey odpowiada za odczytanie wciœniêtego klawisza zdarzenia event w kontekœcie biblioteki Allegro. 
Przyjmuje wskaŸnik keyUpCheck na zmienn¹ logiczn¹ sprawdzaj¹c¹, czy klawisz zosta³ zwolniony, oraz wskaŸnik keyPressed na zmienn¹ przechowuj¹c¹ wartoœæ odczytanego klawisza.
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