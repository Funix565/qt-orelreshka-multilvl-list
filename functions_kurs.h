#ifndef FUNCTIONS_KURS_H
#define FUNCTIONS_KURS_H

#include <types_kurs.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

void** InitArr    ();                                                                   // iнiцiалiзацiя
void ExpandArrPtr (void** &Start, int &SizeArr, int Cnt);                               // розширення
void ShowAll      (void **Start, Ui::MainWindow *ui);                                   // виведення

void addToSort (void** &Start, void* PNew, int posAdd);                                 // додавання у масив вказiвникiв
void zamInSort (void **&Start, QString key, int posCur, Comparator PFunc);              // впорядкування пiсля редагування

// Ф-цiї видалення пiдрiвнiв
void delAll     (void **&Start);
void delCtryLvl (void **&Start);
void delEpsLvl  (void **&Start);

void DelElList  (void **&Start, int Pos);                                               // зсув елементiв у масивi вказiвникiв
void FindElList (void **Start, QString key, int &Pos, bool &isFound, Comparator func);  // пошук половинним дiленням

// Ф-цiї пошуку
void SearchEps  (void **Start, Ui::MainWindow *ui, int posCont, int posCtr, int posEps);
void SearchCtry (void **Start, Ui::MainWindow *ui, int posCont, int posCtr);
void SearchCont (void **Start, Ui::MainWindow *ui, int posCont);

// Ф-цiї порiвняння
int compCont    (void *p, QString key);
int compCountry (void *p, QString key);
int compEps     (void *p, QString key);

// Iндивiдуальнi функцiї
void MinMaxSum (void **Start, Ui::MainWindow *ui);                                      // мiнiмальна та максимальна сума на континентах
void CtrRate   (void **Start, Ui::MainWindow *ui);                                      // рейтинг країн за кiлькiстю випускiв
void FindHost  (void **Start, Ui::MainWindow *ui, QString keyHost);                     // пошук випускiв, де вказаний ведучий має ЗК

// Робота з файлом
void saveINFile   (void **Start, QString path);                                         // збереження
void openFROMFile (void **&Start, QString path);                                        // вiдтворення


#endif // FUNCTIONS_KURS_H
