#include <types_kurs.h>
#include <functions_kurs.h>

/* Ф-цiя видалення всiх рiвнiв */
void delAll(void **&Start)
{
    int cnt = ((int*)Start)[POS_CNT];
    // Цикл видалення пiдрiвнiв країн
    for (int j = 0; j < cnt; j++)
    {
        delCtryLvl(((TCont*)Start[j])->sublvl);
        void *delEL = Start[j];
        delete (TCont*) delEL;   // Видалення континету
    }
    cnt = 0;    // Тепер список порожнiй
    ((int*)Start)[POS_CNT] = cnt;
}

/* Ф-цiя видалення пiдрiвня країн. 2-ий рiвень */
void delCtryLvl (void **&Start)
{
    int cnt = ((int*)Start)[POS_CNT]; // К-сть країн
    for (int j = 0; j < cnt; j++)
    {
        delEpsLvl(((TCountry*)Start[j])->sublvl); // видалення пiдрiвня випускiв для кожної країни
        void *delEl = Start[j];
        delete (TCountry*) delEl;
    }
}
/* Ф-цiя видалення пiдрiвня випускiв. 3-iй рiвень */
void delEpsLvl(void **&Start)
{
    int cnt = ((int*)Start)[POS_CNT];   // К-сть епiзодiв
    for (int j = 0; j < cnt; j++)
    {
        void *delEl = Start[j];
        delete (TEpisode*) delEl;
    }
}

/* Ф-цiя зсуву елементiв у масивi вказiвникiв */
void DelElList (void **&Start, int PosDel)
{
    int cnt = ((int*)Start)[POS_CNT];   // К-сть елементiв

    if (PosDel == (cnt - 1))    // Якщо видаляємо останнiй
    {
        ((int*)Start)[POS_CNT] = cnt - 1; // Зменшуємо к-сть елементiв
        return;
    }
    // Цикл зсуву
    for (int i = PosDel; i < cnt - 1; i++)
    {
        Start[i] = Start[i + 1];
    }

    ((int*)Start)[POS_CNT] = cnt - 1;  // Оновлення даних про к-сть елементiв
}
