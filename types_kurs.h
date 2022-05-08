#ifndef TYPES_KURS_H
#define TYPES_KURS_H

#include <QString>
#include <QDate>

#define POS_CNT        -1   // iндекс масива, де зберiгається значення count (к-сть елементiв)
#define POS_SZ         -2   // iндекс масива, де зберiгається значення size  (макс. к-сть)
#define SIZE_ARR_FIRST  6   // контстанта початковго розмiру масиву
#define DELTA           5   // к-сть елементiв, на яку буде збiльшуватися розмiр масиву у разi розширення

typedef int (*Comparator) (void *ptr, QString key); // Вказiвник на функцiю порiвняння

typedef struct              // опис типу структури для Кореня - програми
{
    QString      name;      // назва програми
    int          ses_count; // к-сть епiзодiв
    void **      sublvl;    // вказiвник на пiдрiвень
}TRoot;

typedef struct              // опис типу стркуктри для 1 рiвня - Континент
{
    QString      name;      // назва
    unsigned int square;    // площа
    double       landmass;  // вiдсоток сушi
    void**       sublvl;    // вказiвник на пiдрiвень
} TCont;

typedef struct              // опис типу структури для 2 рiвня - Країна
{
    QString       name;     // назва
    QString       capital;  // столиця
    int           numOfEpisodes; // к-сть випускiв
    void**        sublvl;   // вказiвник на пiдрiвень
} TCountry;

typedef struct              // опис типу структури для 3 рiвня - Випуск
{
    QDate date;             // дата виходу в ефiр
    QString city;           // вiдвiдане мiсто
    QString fio1;           // прiзвище, iм'я першого ведучого
    QString fio2;           // прiзвище, iм'я другого ведучого
    bool card1;             // наявнiсть карти у першого
    bool card2;             // наявнiсть карти у другого
    double SpentSum;        // загальна витрачена сума
} TEpisode;

typedef struct MinMaxCont   // Опис типу структури для iндивiдуального завдання
{
    QString Cont_name;      // назва континенту
    QString Ctr_name;       // назва країни
    QString City_name;      // вiдвiдане мiсто
    double SpentSum;        // витрачена сума
    QDate dateEps;          // дата виходу в ефiр
    int EpsNum;             // к-сть випускiв
    QString fio;            // ведучий
    bool card;              // карта
    MinMaxCont *next;       // наступний елемент
} TMinMax;




#endif // TYPES_KURS_H
