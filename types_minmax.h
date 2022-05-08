#ifndef TYPES_MINMAX_H
#define TYPES_MINMAX_H
#include <types_kurs.h>

// Ф-цiї для роботи з лiнiйним списком
void initMinMax (TMinMax *&Start);                          // iнiцiалiзацiя
void addToStartHost (TMinMax *&Start, TMinMax *PNew);       // додавання в початок
void addToSortRateList (TMinMax *&Start, TMinMax *PNew);    // додавання в упорядкований
void delList (TMinMax* &Start);                             // видалення списку

#endif // TYPES_MINMAX_H
