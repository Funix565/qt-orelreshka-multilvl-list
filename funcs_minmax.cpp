#include <types_kurs.h>
#include <types_minmax.h>
#include <QDebug>

/* Ф-цiя iнiцiалiзацiї лiнiйного списку*/
void initMinMax(TMinMax *&Start)
{
    Start = NULL;
}

void addToStartHost (TMinMax *&Start, TMinMax *PNew)
{
    PNew->next = Start;
    Start = PNew;
}

/* Ф-цiя додавання елемента в упорядкований лiнiйний список
   За спаданням. Вiд бiльшої кiлькостi випускiв до меншої. */
void addToSortRateList(TMinMax *&Start, TMinMax *PNew)
{
    TMinMax *pp, *wp;   // допомiжнi вказiвники
    wp = Start;         // починаємо з першого
    pp = NULL;          // попереднього у першого немає

    // цикл. Поки елемент iснує i поки ключове поле менше, нiж у поточного
    while (wp != NULL && PNew->EpsNum < wp->EpsNum)
    {
        pp = wp;        // вказ pp отримує адресу поточного елемента
        wp = wp->next;  // наступний елемент стає поточним
    }
    // wp утримує адресу, перед яким треба вставити новий
    if (pp == NULL)
    {
        // Якщо додаємо в початок
        Start = PNew;   // Першим стає новий
    }
    else // iнакше
    {
        pp->next = PNew; // попереднiй елемент зв'язується з новим
    }
    PNew->next = wp;     // Новий елемент зв'язується з поточним
}

/* Ф-цiя видалення всього списку*/
void delList(TMinMax *&Start)
{
    TMinMax *DelP;            // Вказiвник для збереження адреси
    while (Start != NULL)     // Поки не пройдемо всi елементи
    {
        DelP = Start;         // Елемент для видалення
        Start = Start->next;  // Першим стає наступний
        delete DelP;          // Звiльняємо пам'ять
    }
}
