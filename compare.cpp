#include <types_kurs.h>
#include <functions_kurs.h>

/* Ф-цiї порiвняння елементiв
   Приймають вказ. на початок, ключ для порiвняння
   Повертають службовi значення */
int compCont(void *p, QString key)
{
    if (((TCont*)p)->name == key)
    {
        return 0;
    }
    else
    {
        if (((TCont*)p)->name > key) // континенти за назвою
        {
            return -1;
        }
        else
            return 1;
    }
}

int compCountry(void *p, QString key)
{
    if (((TCountry*)p)->name == key)
    {
        return 0;
    }
    else
    {
        if (((TCountry*)p)->name > key) // країни за назвою
        {
            return -1;
        }
        else
            return 1;
    }
}

int compEps(void *p, QString key)
{
    QDate keyDate = QDate::fromString(key, "dd/MM/yyyy");

    if (((TEpisode*)p)->date == keyDate)
    {
        return 0;
    }
    else
    {
        if (((TEpisode*)p)->date > keyDate) // випуски за датою
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}
